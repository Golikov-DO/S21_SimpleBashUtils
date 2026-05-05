#include <getopt.h>

#include "cat.h"

void ParseCatFlags(int argc, char **argv, CatFlags *flags) {
  int opt;

  while ((opt = getopt(argc, argv, "benstvET")) != -1) {
    switch (opt) {
      case 'b':
        flags->b = true;
        break;
      case 'e':
        flags->e = true;
        flags->v = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 't':
        flags->t = true;
        flags->v = true;
        break;
      case 'v':
        flags->v = true;
        break;
      case 'E':
        flags->e = true;
        break;
      case 'T':
        flags->t = true;
        break;
    }
  }
}