import styles from './page.module.scss'
import Window from "@/components/Window"
import DocumentIconCard from "@/components/DocumentIcon"

export default function Project() {
  return (
    <Window title="Projects">
      <DocumentIconCard url="/risc-v-emulator" text="RISC-V" />
    </Window>
  );
}
