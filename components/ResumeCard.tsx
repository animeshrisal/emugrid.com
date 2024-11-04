import { ReactNode } from "react";
import styles from './ResumeCard.module.scss';

type ResumeCardProps = {
  position: string;
  company: string;
  location: string;
  date: string;
  children: ReactNode;
}

const ResumeCard: React.FunctionComponent<ResumeCardProps> = (props) => {
  const { position, location, company, date, children } = props;

  return (
    <div className={styles.card}>
      <div className={styles.header}>
        <div>
          <div>{position}</div>
          <div className={styles["header-left"]}>
            <span className={styles["header-company"]}>{company}</span>-<span
              className={styles["header-location"]}>{location}</span>
          </div>
        </div>
        {date}
      </div>
      {children}
    </div>
  )
}

export default ResumeCard;
