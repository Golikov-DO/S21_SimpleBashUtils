#ifndef SRC_GREP_GREP_H_
#define SRC_GREP_GREP_H_

#include <regex.h>
#include <stdbool.h>

typedef struct {
  bool i, v, c, l, n;
  bool h, s, o;
  bool pattern_specified, error;
  char *pattern;
  char **patterns;
  int pattern_count;
} GrepFlags;

void ParseGrepFlags(int argc, char **argv, GrepFlags *flags);
void ProcessGrepFile(const char *filename, const GrepFlags *flags,
                     regex_t *regex, int file_count);
void FreeGrepFlags(GrepFlags *flags);

#endif