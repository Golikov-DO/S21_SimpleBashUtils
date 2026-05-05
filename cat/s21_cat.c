#include <getopt.h>

#include "cat.h"

int main(int argc, char **argv) {
  CatFlags flags = {0};
  CatState state = {1, '\n', 0};  // 🔥 ВАЖНО

  ParseCatFlags(argc, argv, &flags);

  if (optind == argc) {
    ProcessCatFile(NULL, &flags, &state);
  } else {
    for (int i = optind; i < argc; i++) {
      ProcessCatFile(argv[i], &flags, &state);
    }
  }

  return 0;
}