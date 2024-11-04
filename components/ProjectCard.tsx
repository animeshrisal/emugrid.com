
type ProjectCardProps = {
  test: string;
}

const ProjectCard: React.FunctionComponent<ProjectCardProps> = props => {
  const { test } = props;

  return (
    <div>{test}</div>
  )

}

export default ProjectCard;
