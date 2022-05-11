import styles from "../styles/TacoList.module.css";
import TacoCard from "./TacoCard";

const TacoList = ({ tacoList }) => {
  return (
    <div className={styles.container}>
      <h1 className={styles.title}>THE BEST TAKOS IN TOWN</h1>
      <p className={styles.desc}>
      </p>
      <div className={styles.wrapper}>
        {tacoList.map((taco) => (
          <TacoCard key={taco._id} taco={taco} />
        ))}
      </div>
    </div>
  );
};

export default TacoList;