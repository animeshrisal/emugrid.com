#include "cpu.h"
#include "disassemble.h"
#include "elf.h"
#include "stdio.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

CPU *cpu_instance = NULL;
Bus bus;
uint8 *text_section;
int text_size = 0;
int pc = 0;
int instruction_number = 0;
int number_of_instructions = 0;

void allocate_CPU() { cpu_instance = (CPU *)malloc(sizeof(CPU)); };

CPU *get_cpu_ptr() { return cpu_instance; }

void free_cpu() {
  free(cpu_instance);
  cpu_instance = NULL;
}

struct shdr read_elf_header(FILE *f) {
  struct elfhdr ehdr;
  if (fread(&ehdr, sizeof(ehdr), 1, f) != 1) {
    perror("Failed to read ELF header");
    fclose(f);
    exit(EXIT_FAILURE);
  }

  if (ehdr.magic[0] == ELF_MAGIC_0 && ehdr.magic[1] == ELF_MAGIC_1 &&
      ehdr.magic[2] == ELF_MAGIC_2 && ehdr.magic[3] == ELF_MAGIC_3) {
    printf("The file is a valid ELF file.\n");
  } else {
    printf("The file is not a valid ELF file.\n");
  }

  fseek(f, ehdr.shoff, SEEK_SET);

  struct shdr *sections = malloc(ehdr.shnum * sizeof(struct shdr));
  int i = fread(sections, sizeof(struct shdr), ehdr.shnum, f);

  char *shstrtab = NULL;
  struct shdr shstrtab_header = sections[ehdr.shstrndx];

  fseek(f, shstrtab_header.offset, SEEK_SET);
  shstrtab = malloc(shstrtab_header.size);
  fread(shstrtab, shstrtab_header.size, 1, f);

  for (int i = 0; i < ehdr.shnum; i++) {
    const char *section_name = shstrtab + sections[i].name;
    if (strcmp(section_name, ".text") == 0) {
      return sections[i];
    }
  }
};

int read_elf_file(const char *elf_file) {
  cpu_instance->bus = &bus;
  FILE *file = fopen(elf_file, "r");
  int quit = 0;

  struct shdr text = read_elf_header(file);
  fseek(file, text.offset, SEEK_SET);
  text_section = malloc(text.size + 4);
  text_size = text.size;
  if (!text_section) {
    perror("Failed to allocate memory");
    fclose(file);
    exit(1);
  }

  fread(text_section, 1, text.size, file);
  fclose(file);

  return 1;
}

int handle_instruction() {
  uint32 instruction = *(uint32 *)(text_section + cpu_instance->pc);
  run_instruction(cpu_instance, instruction);
  return cpu_instance->pc / 4;
}

char *show_disassembled_code() {
  static char buffer[1000];
  memset(buffer, 0, sizeof(buffer));

  int no_of_instructions = text_size / 4;
  for (int i = 0; i < no_of_instructions; i++) {
    uint32 instruction = *(uint32 *)(text_section + i * 4);
    char *disassembled = disassemble_instruction(instruction);
    strncat(buffer, disassembled, sizeof(buffer) - strlen(buffer) - 1);
    strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);
  }

  return buffer;
}
