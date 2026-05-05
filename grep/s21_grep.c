#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "grep.h"

int main(int argc, char **argv) {
  GrepFlags flags = {0};

  ParseGrepFlags(argc, argv, &flags);

  if (flags.error) {
    FreeGrepFlags(&flags);
    return 1;
  }

  if (flags.pattern_count == 0) {
    fprintf(stderr, "No pattern\n");
    FreeGrepFlags(&flags);
    return 1;
  }

  regex_t *regexes = malloc(sizeof(regex_t) * flags.pattern_count);
  if (!regexes) {
    fprintf(stderr, "Memory allocation error\n");
    FreeGrepFlags(&flags);
    return 1;
  }

  int reg_flags = REG_EXTENDED;
  if (flags.i) reg_flags |= REG_ICASE;

  for (int i = 0; i < flags.pattern_count; i++) {
    if (regcomp(&regexes[i], flags.patterns[i], reg_flags) != 0) {
      fprintf(stderr, "Regex error\n");

      for (int j = 0; j < i; j++) {
        regfree(&regexes[j]);
      }
      free(regexes);
      FreeGrepFlags(&flags);

      return 1;
    }
  }

  int file_count = argc - optind;

  if (file_count == 0) {
    ProcessGrepFile(NULL, &flags, regexes, 1);  // 🔥 stdin
  } else {
    for (int i = optind; i < argc; i++) {
      ProcessGrepFile(argv[i], &flags, regexes, file_count);
    }
  }

  for (int i = 0; i < flags.pattern_count; i++) {
    regfree(&regexes[i]);
  }

  free(regexes);
  FreeGrepFlags(&flags);
  return 0;
}