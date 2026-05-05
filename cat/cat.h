#ifndef SRC_CAT_CAT_H_
#define SRC_CAT_CAT_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool v;
} CatFlags;

typedef struct {
  int line_number;
  int prev_char;
  int empty_line_count;
} CatState;

void ParseCatFlags(int argc, char **argv, CatFlags *flags);
void ProcessCatFile(const char *filename, const CatFlags *flags,
                    CatState *state);

#endif  // SRC_CAT_CAT_H_