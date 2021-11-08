import { useState } from "react";
import styled from "styled-components";
import { login } from "../redux/apiCalls";
import { mobile } from "../responsive";
import { useDispatch, useSelector } from "react-redux";


const Container = styled.div`
  width: 100vw;
  height: 100vh;
  background: linear-gradient(
      rgba(255, 255, 255, 0.5),
      rgba(255, 255, 255, 0.5)
    ),
    url("data:image/jpeg;base64,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")
      center;
  background-size: cover;
  display: flex;
  align-items: center;
  justify-content: center;
`;

const Wrapper = styled.div`
  width: 25%;
  padding: 20px;
  background-color: white;
  ${mobile({ width: "75%" })}
`;

const Title = styled.h1`
  font-size: 24px;
  font-weight: 300;
`;

const Form = styled.form`
  display: flex;
  flex-direction: column;
`;

const Input = styled.input`
  flex: 1;
  min-width: 40%;
  margin: 10px 0;
  padding: 10px;
`;

const Button = styled.button`
  width: 40%;
  border: none;
  padding: 15px 20px;
  background-color: teal;
  color: white;
  cursor: pointer;
  margin-bottom: 10px;
  &:disabled {
    color: green;
    cursor: not-allowed;
  }
`;

const Link = styled.a`
  margin: 5px 0px;
  font-size: 12px;
  text-decoration: underline;
  cursor: pointer;
`;

const Error = styled.span`
  color: red;
`;

const Login = () => {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const dispatch = useDispatch();
  const { isFetching, error } = useSelector((state) => state.user);

  const handleClick = (e) => {
    e.preventDefault();
    login(dispatch, { username, password });
  };
  return (
    <Container>
      <Wrapper>
        <Title>SIGN IN</Title>
        <Form>
          <Input
            placeholder="username"
            onChange={(e) => setUsername(e.target.value)}
          />
          <Input
            placeholder="password"
            type="password"
            onChange={(e) => setPassword(e.target.value)}
          />
          <Button onClick={handleClick} disabled={isFetching}>
            LOGIN
          </Button>
          {error && <Error>Something went wrong...</Error>}
          <Link>DO NOT YOU REMEMBER THE PASSWORD?</Link>
        </Form>
      </Wrapper>
    </Container>
  );
};

export default Login;