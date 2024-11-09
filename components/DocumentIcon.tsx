import styles from "./DocumentIcon.module.scss"

type DocumentIconProps = {
  text: string;
}

const DocumentIconCard: React.FunctionComponent<DocumentIconProps> = props => {
  
  const { text } = props;

  return (
      <a href="/resume"
      className={styles["document-container"]}> 
        <div className={styles.folder}></div>
        <div className={styles.text}>{text}</div>
      </a>
  )
}

export default DocumentIconCard;


