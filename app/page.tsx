import styles from './page.module.scss';
import DocumentIconCard from "@/components/DocumentIcon"
import Window from "@/components/Window"
import FileExplorer from "@/components/FileExplorer"

export default function Home() {
  return (
    <div className={styles.container}>
      <h1 className='header'>Emugrid OS v0.1</h1>
      <div>
        <Window title="About me">
          <div className={styles.notepad}>
            <p>
              I&lsquo;m currently a Masters student studying International Information System Science at Otto-Friedrich-Universität Bamberg.
            </p>
            <p>
              I also work part time as a Software Engineer for Lion5 GmbH. I currently have 5 years of professional experience working as a full stack web developer.
            </p>
            <br />
            <p>Languages I use: TypeScript, SQL, C++, Python</p>
            <div>Some of my interests
              Emulation, Retro Games, Pixel Art and Cloud Computing.
              <span className={styles["blinking-block"]}></span>
            </div>
          </div>
        </Window>

        <Window title="My computer">
          <FileExplorer>
            <DocumentIconCard url="/resume" text="Resume" />
            <DocumentIconCard url="/projects" text="Projects" />
            <DocumentIconCard url="/blog" text="Blogs" />
          </FileExplorer>
        </Window>
      </div>
    </div>
  );
}
