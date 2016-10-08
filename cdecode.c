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
  char * arraychars;
  if ((argc == 1) || (!(strcmp(argv[1], "-")))) {
    arraychars = malloc(sizeof(char) * 32 * 1024);
    fgets(arraychars, 32*1024, stdin);
  } else {
    uint32_t fsize;
    FILE* file = fopen(argv[1], "rb");
    if (file == NULL) {
      fputs("Failed to open input file!\n", stderr);
      exit(2);
    }
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);
    arraychars = malloc(sizeof(char) * fsize);
    fgets(arraychars, fsize, file);
    fclose(file);
  }
  char * nextword;
  char tmpword[128];
  nextword = strtok(arraychars, " ");
  strcpy(tmpword, nextword);
  for (i = 0;i < 65535;i++) {
    if (!(strcmp(wordarray[i], tmpword))) break;
  }
  uint16_t fsize = i;
  if (fsize % 2) {
    nextword = strtok(NULL, " ");
    strcpy(tmpword, nextword);
    for (i = 0;i < 65535;i++) {
      if (!(strcmp(wordarray[i], tmpword))) {
        fwrite(&i, 1, 1, stdout);
        break;
      }
    }
  }
  nextword = strtok(NULL, " \n");
  while (nextword != NULL) {
    strcpy(tmpword, nextword);
    for (i = 0;i < 65535;i++) {
      if (!(strcmp(wordarray[i], tmpword))) {
        fwrite(&i, sizeof(short), 1, stdout);
        break;
      }
    }
    nextword = strtok(NULL, " \n");
  }
  free(arraychars);
  return 0;
}
