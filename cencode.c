#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char * argv[]) {
  FILE* wordsfile;
  if (argc == 1) {
    wordsfile = fopen("wordlist", "rb");
  } else if (argc < 4) { 
    if (argc == 3) {
      wordsfile = fopen(argv[argc-1], "rb");
    } else {
      wordsfile = fopen("wordlist", "rb");
    }
  } else {
      fputs("Too many arguments.\n", stderr);
      exit(1);
  }
  if (wordsfile == NULL) {
    fputs("Error opening wordlist file.\n", stderr);
    exit(2);
  }
  char wordnew[128];
  fgets(wordnew, 100, wordsfile);
  uint32_t i = 0;
  while (!feof(wordsfile)) {
    i++;
    fscanf(wordsfile, "%100s\n", wordnew);
  }
  rewind(wordsfile);
  if (i < 65536) {
    fputs("Error: wordlist must contain at least 65536 words.\n", stderr);
    exit(1);
  }
  char **wordarray = malloc(sizeof(char *) * 65536);
  for (i = 0;i < 65535;i++) {
    fscanf(wordsfile, "%100s\n", wordnew);
    wordarray[i] = (char *)malloc(sizeof(char) * (strlen(wordnew) + 1));
    strcpy(wordarray[i], wordnew);
  }
  fclose(wordsfile);
  uint16_t fsize;
  char * arraychars;
  if ((argc == 1) || (!(strcmp(argv[1], "-")))) {
    arraychars = malloc(sizeof(char) * 65535);
    fsize = fread(arraychars, 1, 65535, stdin);
  } else {
    FILE* file = fopen(argv[1], "rb");
    if (file == NULL) {
      fputs("Failed to open input file!\n", stderr);
      exit(2);
    }
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);
    arraychars = malloc(sizeof(char) * fsize);
    fread(arraychars, 1, fsize, file);
    fclose(file);
  }
  uint32_t offset = 0;
  printf("%s ", wordarray[fsize]);
  if (fsize % 2) {
    printf("%s ", wordarray[*((uint8_t *) (&arraychars[offset]))]);
    fsize = fsize - 1;
    offset = 1;
  }
  printf("%s", wordarray[*((uint16_t *) (&arraychars[offset]))]);
  offset += 2;
  for (i = offset;i < fsize;i += 2) {
    printf(" ");
    printf("%s", wordarray[*((uint16_t *) (&arraychars[i]))]);
  }
  printf("\n");
  free(arraychars);
  free(wordarray);
  return 0;
}
