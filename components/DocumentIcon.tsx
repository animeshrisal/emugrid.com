import styles from "./DocumentIcon.module.scss"

type DocumentIconProps = {
  text: string;
  url: string;
}

const DocumentIconCard: React.FunctionComponent<DocumentIconProps> = props => {

  const { text, url } = props;

  return (
    <a href={url}
      className={styles["document-container"]}>
      <div className={styles.folder}></div>
      <div className={styles.text}>{text}</div>
    </a>
  )
}

export default DocumentIconCard;


