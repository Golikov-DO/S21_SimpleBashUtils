#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/utils.h"
#include "grep.h"

static int MatchAny(const char *line, const GrepFlags *flags,
                    regex_t *regexes) {
  for (int i = 0; i < flags->pattern_count; i++) {
    if (regexec(&regexes[i], line, 0, NULL, 0) == 0) return 1;
  }
  return 0;
}

static void PrintPrefix(const char *filename, int line_number,
                        const GrepFlags *flags, int file_count) {
  if (!flags->h && file_count > 1) {
    printf("%s:", filename);
  }
  if (flags->n) {
    printf("%d:", line_number);
  }
}

static void PrintMatchesOnly(const char *line, regex_t *regex,
                             const GrepFlags *flags, const char *filename,
                             int line_number, int file_count) {
  regmatch_t match;
  const char *ptr = line;

  while (regexec(regex, ptr, 1, &match, 0) == 0) {
    PrintPrefix(filename, line_number, flags, file_count);

    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(ptr[i]);
    }
    putchar('\n');

    ptr += match.rm_eo;
  }
}

void ProcessGrepFile(const char *filename, const GrepFlags *flags,
                     regex_t *regexes, int file_count) {
  FILE *file = stdin;

  if (filename) {
    file = fopen(filename, "r");
    if (!file) {
      if (!flags->s) PrintError("s21_grep", filename);
      return;
    }
  }

  char *line = NULL;
  size_t len = 0;
  int line_number = 0;
  int match_count = 0;

  while (getline(&line, &len, file) != -1) {
    line_number++;

    int match = MatchAny(line, flags, regexes);
    if (flags->v) match = !match;
    if (!match) continue;

    match_count++;

    if (flags->l) break;
    if (flags->c) continue;

    if (flags->o && !flags->v) {
      for (int i = 0; i < flags->pattern_count; i++) {
        PrintMatchesOnly(line, &regexes[i], flags, filename, line_number,
                         file_count);
      }
      continue;
    }

    PrintPrefix(filename, line_number, flags, file_count);
    printf("%s", line);
  }

  if (flags->c) {
    if (filename && !flags->h && file_count > 1) {
      printf("%s:", filename);
    }
    printf("%d\n", match_count);
  }

  if (flags->l && match_count > 0) {
    printf("%s\n", filename);
  }

  free(line);
  fclose(file);
}