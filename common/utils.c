#include "utils.h"

#include <errno.h>
#include <string.h>

void PrintError(const char *program, const char *filename) {
  fprintf(stderr, "%s: %s: %s\n", program, filename, strerror(errno));
}