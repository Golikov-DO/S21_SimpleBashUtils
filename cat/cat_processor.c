#include <stdio.h>

#include "../common/utils.h"
#include "cat.h"

static void PrintLineNumber(const CatFlags *flags, CatState *state, int c) {
  if (state->prev_char == '\n') {
    if (flags->b) {
      if (c != '\n') {
        printf("%6d\t", state->line_number++);
      }
    } else if (flags->n) {
      printf("%6d\t", state->line_number++);
    }
  }
}

static int SkipEmptyLines(const CatFlags *flags, CatState *state, int c) {
  if (!flags->s) return 0;

  if (c == '\n') {
    state->empty_line_count++;

    if (state->empty_line_count > 2) {
      return 1;
    }
  } else {
    state->empty_line_count = 0;
  }

  return 0;
}

static void PrintChar(const CatFlags *flags, int c) {
  if (flags->t && c == '\t') {
    printf("^I");
    return;
  }

  if (flags->e && c == '\n') {
    printf("$");
  }

  if (flags->v && c < 32 && c != '\n' && c != '\t') {
    printf("^%c", c + 64);
    return;
  }

  putchar(c);
}

void ProcessCatFile(const char *filename, const CatFlags *flags,
                    CatState *state) {
  FILE *file = stdin;

  if (filename) {
    file = fopen(filename, "r");
    if (!file) {
      PrintError("s21_cat", filename);
      return;
    }
  }

  int c;
  while ((c = fgetc(file)) != EOF) {
    if (SkipEmptyLines(flags, state, c)) continue;

    PrintLineNumber(flags, state, c);
    PrintChar(flags, c);

    state->prev_char = c;
  }

  if (filename) fclose(file);
}