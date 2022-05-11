import Image from "next/image";
import styles from "../styles/TacoCard.module.css";
import Link from "next/link";

const TacoCard = ({ taco }) => {
  return (
    <div className={styles.container}>
      <Link href={`/product/${taco._id}`} passHref>
        <Image src={taco.img} alt="" width="500" height="500" />
      </Link>
      <h1 className={styles.title}>{taco.title}</h1>
      <span className={styles.price}>${taco.prices[0]}</span>
      <p className={styles.desc}>{taco.desc}</p>
    </div>
  );
};

export default TacoCard;