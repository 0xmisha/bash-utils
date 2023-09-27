#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

enum Flags {
  NONE = 0,
  FLAG_B = 1,
  FLAG_E = 1 << 1,
  FLAG_N = 1 << 2,
  FLAG_S = 1 << 3,
  FLAG_T = 1 << 4,
  FLAG_V = 1 << 5
};

int argParse(int argc, char* argv[]);
void printFile(FILE* file, int mask);
