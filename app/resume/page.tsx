
import ResumeCard from "@/components/ResumeCard";
import styles from './page.module.scss';
import Window from "@/components/Window"

export default function Resume() {
  return (
    <Window title="Work Experience">
      <div className={styles['container']}>
        <div>
          <ResumeCard
            date="07.2023 - Present"
            position="Working Student - Software Engineer"
            company="Lion5"
            location="Bamberg, Bavaria, Germany"
          >
            <ul>
              <li>
                Create frontend components using VueJS and Web APIs using Spring Boot
              </li>
            </ul>
          </ResumeCard>
          <ResumeCard
            date="07.2022 - 12.2023"
            position="Frontend Engineer"
            company="Angelswing"
            location="Seoul, South Korea (Remote)"
          >
            <ul>
              <li>
                Implement user interface for permission control, user invitation,
                image gallery, auto saving image to the server and creating shareable
                links.
              </li>
              <li>
                Enabled users to create, manipulate, and share 3D box models, along
                with a UI to control the box models using React, Redux, and a 3D GIS
                Framework.
              </li>
              <li>
                Created user interface that allows users to upload and interact with
                their custom made 3D Models.
              </li>
              <li>
                Modified a generic gizmo that allows users to rotate and translate in
                the Z-Axis in the same mode.
              </li>
            </ul>
          </ResumeCard>
          <ResumeCard
            date="01.2022 - 06.2022"
            position="Part-time Morning Lecturer"
            company="Orchid International College"
            location=" Kathmandu, Nepal"
          >
            <ul>
              <li>
                Taught Software Engineering to BCA 4th semester students with topics
                included as Documentation, SDLC Models, Testing and Project
                Management.
              </li>
              <li>
                Taught Django Web Development to bachelors degree students as an extra
                course covering Python, Web Development and JavaScript Integration.
              </li>
            </ul>
          </ResumeCard>
          <ResumeCard
            date="07.2019 -  04.2022"
            position="Software Engineer"
            company="Bajra Technologies"
            location=" Kathmandu, Nepal"
          >
            <ul>
              <li>
                Lead a team of engineers developing an AWS visualization software
                using Angular and Serverless Architecture with TypeScript.
              </li>
              <li>
                Lead a team of engineers developing a SaaS application that helps
                track fostering processes and taking care of pets for animal rescue
                organizations.
              </li>
              <li>
                Optimized a React Native application by speeding up screen response
                and minimizing memory usage.
              </li>
              <li>
                Enhanced efficiency of a NLP backend system written in Django by
                implementing caching, filtering, and pagination techniques resulting
                in reduced network response times and system size.
              </li>
            </ul>
          </ResumeCard>
          <ResumeCard
            date="07.2019 - 08.2019"
            position="Instructor"
            company="Orchid International College"
            location="Kathmandu, Nepal"
          >
            <ul>
              <li>
                Taught a 30 hour course about Web Development with Python and Flask to
                bachelor degree students.
              </li>
            </ul>
          </ResumeCard>
          <ResumeCard
            date="11.2018 - 06.2019"
            position="Internship"
            company="Yellow Nepal"
            location="Kathmandu, Nepal"
          >
            <ul>
              <li>Internship as a requirement for my final semester.</li>
              <li>Worked as a react native developer developing UI components.</li>
              <li>Researched Flutter Framework</li>
            </ul>
          </ResumeCard>
        </div>
      </div>
    </Window>
  )
}
