import styles from './page.module.css';
import DocumentIconCard from "@/components/DocumentIcon.tsx"

export default function Home() {
  return (
    <div className={styles.container}>
      <h1 className='header'>Emugrid OS v0.1</h1>
      <h1>Hi, I&lsquo;m Animesh Risal.</h1>
      <br />
      <h2>Welcome to my website. You can find some of my info here</h2>

      <br />
      <div>
        <h1>About me</h1>
        <br />
        <p>
          I&lsquo;m currently a Masters student studying International Information System Science at Otto-Friedrich-Universität Bamberg.
          <br />
          I also work part time as a Software Engineer for Lion5 GmbH. I currently have 5 years of professional experience working as a full stack web developer.
        </p>
        <br />

        <p>Languages I use: TypeScript, SQL, C++, Python</p>
        <br />
        <p>Some of my interests
        Emulation, Retro Games, Pixel Art and Cloud Computing
      </p> 
      <div>
      <br />
      <h2>Contact</h2> 
        <span>Email: emugrid@proton.me</span><span className={styles["blinking-block"]}></span>
      <br />
      </div>
      <div className={styles.window}>
          <div className={styles['window-header']}>My Computer</div>
          <div className={styles["window-container"]}>
            <DocumentIconCard text="Resume" />  
            <DocumentIconCard text="Projects" />
            <DocumentIconCard text="Blogs" />
          </div>
          </div>
      </div>
    </div>
  );
}
