import { useSelector } from "react-redux";
import styled from "styled-components";
import Footer from "../components/Footer";
import Navbar from "../components/Navbar";
import { mobile } from "../responsive";
import StripeCheckout from "react-stripe-checkout";
import { useEffect, useState } from "react";
import { userRequest } from "../requestMethods";
import { useHistory } from "react-router";

const KEY = process.env.REACT_APP_STRIPE;

const Container = styled.div``;

const Wrapper = styled.div`
  padding: 20px;
  ${mobile({ padding: "10px" })}
`;

const Title = styled.h1`
  font-weight: 300;
  text-align: center;
`;

const Bottom = styled.div`
  display: flex;
  justify-content: space-between;
  ${mobile({ flexDirection: "column" })}
`;

const Info = styled.div`
  flex: 3;
`;

const Product = styled.div`
  display: flex;
  justify-content: space-between;
  ${mobile({ flexDirection: "column" })}
`;

const ProductDetail = styled.div`
  flex: 2;
  display: flex;
`;

const Image = styled.img`
  width: 200px;
`;

const Details = styled.div`
  padding: 20px;
  display: flex;
  flex-direction: column;
  justify-content: space-around;
`;

const ProductName = styled.span``;

const ProductId = styled.span``;

const PriceDetail = styled.div`
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
`;

const ProductAmountContainer = styled.div`
  display: flex;
  align-items: center;
  margin-bottom: 20px;
`;

const ProductAmount = styled.div`
  font-size: 24px;
  margin: 5px;
  ${mobile({ margin: "5px 15px" })}
`;

const ProductPrice = styled.div`
  font-size: 30px;
  font-weight: 200;
  ${mobile({ marginBottom: "20px" })}
`;

const Hr = styled.hr`
  background-color: #eee;
  border: none;
  height: 1px;
`;

const Summary = styled.div`
  flex: 1;
  border: 0.5px solid lightgray;
  border-radius: 10px;
  padding: 20px;
  height: 50vh;
`;

const SummaryTitle = styled.h1`
  font-weight: 200;
`;

const SummaryItem = styled.div`
  margin: 30px 0px;
  display: flex;
  justify-content: space-between;
  font-weight: ${(props) => props.type === "total" && "500"};
  font-size: ${(props) => props.type === "total" && "24px"};
`;

const SummaryItemText = styled.span``;

const SummaryItemPrice = styled.span``;

const Button = styled.button`
  width: 100%;
  padding: 10px;
  background-color: black;
  color: white;
  font-weight: 600;
`;

const Cart = () => {
  const cart = useSelector((state) => state.cart);
  const [stripeToken, setStripeToken] = useState(null);
  const history = useHistory();

  const onToken = (token) => {
    setStripeToken(token);
  };

  useEffect(() => {
    const makeRequest = async () => {
      try {
        const res = await userRequest.post("/checkout/payment", {
          tokenId: stripeToken.id,
          amount: cart.total * 100,
        });
        history.push("/success", {
          stripeData: res.data,
          products: cart, });
      } catch {}
    };
    stripeToken && makeRequest();
  }, [stripeToken, cart.total, history]);

  return (
    <Container>
      <Navbar />
      <Wrapper>
        <Title>YOUR BAG</Title>
        <Bottom>
          <Info>
          {cart.products.map((product) => (
            <Product>
              <ProductDetail>
                <Image src={product.img} />
                <Details>
                  <ProductName>
                    <b>Product:</b> {product.title}
                  </ProductName>
                  <ProductId>
                    <b>ID:</b> {product._id}
                  </ProductId>
                </Details>
              </ProductDetail>
              <PriceDetail>
                <ProductAmountContainer>
                  <ProductAmount>Qty. {product.quantity}</ProductAmount>
                  </ProductAmountContainer>
                  <ProductPrice>
                    $ {product.price * product.quantity}mxn
                  </ProductPrice>
              </PriceDetail>
            </Product>
          ))}
          <Hr />
          </Info>
          <Summary>
            <SummaryTitle>ORDER SUMMARY</SummaryTitle>
            <SummaryItem>
              <SummaryItemText>Subtotal</SummaryItemText>
              <SummaryItemPrice>$ {cart.total}mxn</SummaryItemPrice>
            </SummaryItem>
            <SummaryItem>
              <SummaryItemText>Estimated Shipping</SummaryItemText>
              <SummaryItemPrice>$ 5.90mxn</SummaryItemPrice>
            </SummaryItem>
            <SummaryItem>
              <SummaryItemText>Shipping Discount</SummaryItemText>
              <SummaryItemPrice>$ -5.90mxn</SummaryItemPrice>
            </SummaryItem>
            <SummaryItem type="total">
              <SummaryItemText>Total</SummaryItemText>
              <SummaryItemPrice>$ {cart.total}mxn</SummaryItemPrice>
            </SummaryItem>
            <StripeCheckout
              name="StoreDB"
              image="data:image/jpeg;base64,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"
              billingAddress
              shippingAddress
              description={`Your total is $${cart.total}mxn`}
              amount={cart.total * 100}
              token={onToken}
              stripeKey="pk_test_51JnzVTElpBU7rwJYz98n3LPWA3bG9xgqfVbFviFDyWrCTDDWPXtOmHdRL2jWlwSb8KMHVDScNoPXA0JDdHkkuFgi00bCcaZ3eu"
            >
              <Button>CHECKOUT NOW</Button>
            </StripeCheckout>
          </Summary>
        </Bottom>
      </Wrapper>
      <Footer />
    </Container>
  );
};

export default Cart;