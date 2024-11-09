import styles from './Window.module.scss';

type WindowProps = {
 title: string;
  children: ReactNode;
}

const Window: React.FunctionComponent<WindowProps> = props => {

  const { title, children } = props;

  return (
    <div className={styles.window}>
      <div className={styles['window-header']}>{ title }</div>
      <div className={styles['window-container']}>
        { children }
      </div>
    </div>
  )
}

export default Window;

