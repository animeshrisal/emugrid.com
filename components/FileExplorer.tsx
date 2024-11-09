import styles from './FileExplorer.module.scss';

type FileExplorerProps = {
  children: ReactNode;
}

const FileExplorer: React.FunctionComponent<FileExplorerProps> = props => {
  const { children } = props;
  return (
    <div className={styles.explorer}>
      { children }
    </div>
  )
}

export default FileExplorer;

