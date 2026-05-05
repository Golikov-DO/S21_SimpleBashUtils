#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grep.h"

static void AddPattern(GrepFlags *flags, const char *pattern) {
  char **tmp =
      realloc(flags->patterns, sizeof(char *) * (flags->pattern_count + 1));

  if (!tmp) {
    flags->error = true;
    return;
  }

  flags->patterns = tmp;
  char *copy = strdup(pattern);
  if (!copy) {
    flags->error = true;
    return;
  }
  flags->patterns[flags->pattern_count++] = copy;
}

static void ReadPatternsFromFile(GrepFlags *flags, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    if (!flags->s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
    }
    flags->error = true;
    return;
  }

  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, file) != -1) {
    line[strcspn(line, "\n")] = '\0';
    AddPattern(flags, line);
  }

  free(line);
  fclose(file);
}

void ParseGrepFlags(int argc, char **argv, GrepFlags *flags) {
  int opt;

  while ((opt = getopt(argc, argv, "e:ivclnhsof:")) != -1) {
    switch (opt) {
      case 'e':
        AddPattern(flags, optarg);
        flags->pattern_specified = true;
        break;
      case 'f':
        ReadPatternsFromFile(flags, optarg);
        flags->pattern_specified = true;
        break;
      case 'i':
        flags->i = true;
        break;
      case 'v':
        flags->v = true;
        break;
      case 'c':
        flags->c = true;
        break;
      case 'l':
        flags->l = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 'h':
        flags->h = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 'o':
        flags->o = true;
        break;
    }
  }

  if (!flags->pattern_specified && optind < argc) {
    AddPattern(flags, argv[optind++]);
  }
}

void FreeGrepFlags(GrepFlags *flags) {
  for (int i = 0; i < flags->pattern_count; i++) {
    free(flags->patterns[i]);
  }
  free(flags->patterns);
}