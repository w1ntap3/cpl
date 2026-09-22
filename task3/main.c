#include "humanize.h"
#include <stdio.h>

#define FILE_PATH "cvut.txt"

int main(int argc, char *argv[]) {
  printf("Humanizing text of \"%s\"...\n", FILE_PATH);
  int words_humanized = humanize_file(FILE_PATH);
  if (words_humanized < 0) {
    fprintf(stderr, "humanize_file() implementation failed.\n");
    return 1;
  }

  printf("\nhumanize_file() implementation altered %d words.\n",
         words_humanized);
  return 0;
}
