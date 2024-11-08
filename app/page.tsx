import styles from './page.module.css';

export default function Home() {
  return (
    <div className={styles.container}>
      <h1>Hi, I'm Animesh Risal.</h1>
      <br />
      <div>
        Contact: emugrid@proton.me
      </div>
      <br />
      <p>Welcome to my website. You can find some of my info here</p>

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
        <p>You can find my resume <u><a href="/resume">here</a></u></p>  
        <br />
        <p>An area of interest I&lsquo;ve picked up over the last few months is RISC-V and 6502 Emulation. </p>
        <br />
      </div>
        <p>Some of my interests
        Emulation, Retro Games, Pixel Art and Cloud Computing
      </p> 
      <br/ >
    </div>
  );
}
