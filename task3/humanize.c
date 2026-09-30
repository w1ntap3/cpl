#include "humanize.h"
#include "synonym.h"

int humanize_file(const char *file_name) {
  int words_humanized = 0;
  char buf[32];
  get_synonym("memory", buf, sizeof(buf));
  return words_humanized;
}
