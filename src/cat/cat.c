#include "cat.h"

int main(int argc, char* argv[]) {
  FILE* file;

  int flags = argParse(argc, argv);
  if (flags != -1) {
    for (int i = (flags == 0 ? 1 : 2); i < argc; i++) {
      if ((file = fopen(argv[i], "r")) == NULL) {
        if (argv[i][0] != '-')
          printf("cat: %s: No such file or directory\n", argv[i]);
      } else {
        printFile(file, flags);
      }
      fclose(file);
    }
  }
  return 0;
}

int argParse(int argc, char* argv[]) {
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", optional_argument, NULL, 'n'},
      {"squeeze-blank", required_argument, NULL, 's'},
  };

  int flags = 0;
  int opt;
  while ((opt = getopt_long(argc, argv, "benstvET", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        flags |= FLAG_B;
        break;
      case 'E':
        flags |= FLAG_E;
        break;
      case 'e':
        flags |= FLAG_E;
        flags |= FLAG_V;
        break;
      case 'n':
        flags |= FLAG_N;
        break;
      case 's':
        flags |= FLAG_S;
        break;
      case 'T':
        flags |= FLAG_T;
        break;
      case 't':
        flags |= FLAG_T;
        flags |= FLAG_V;
        break;
      case '?':
        printf("cat: unrecognized option '%s'\n", argv[optind - 1]);
        return -1;
    }
  }
  return flags;
}

void putcharV(char ch) {
  if ((ch < 32 || ch >= 127) && ch != '\n' && ch != '\t') {
    printf("^%c", ch + 64);
  } else {
    printf("%c", ch);
  }
}

void printFile(FILE* file, int flags) {
  int c;
  int prevC = '\n';
  int strNum = 1;
  int isPrintNeeded = 1;
  int isEmptyLine = 0;
  int isPrevEmptyLine = 0;

  while ((c = fgetc(file)) != EOF) {
    isEmptyLine = (c == '\n' && prevC == '\n') ? 1 : 0;

    if (flags & FLAG_B && !isEmptyLine && prevC == '\n') {
      printf("%6d\t", strNum);
      strNum++;
    } else if (flags & FLAG_N && prevC == '\n' && !(flags & FLAG_B)) {
      printf("%6d\t", strNum);
      strNum++;
    }

    if (flags & FLAG_T && c == '\t') {
      printf("^I");
      isPrintNeeded = 0;
    }

    if (flags & FLAG_E && c == '\n' && !isPrevEmptyLine) {
      printf("$");
    }

    if (flags & FLAG_S && c == '\n' && isPrevEmptyLine) {
      isPrintNeeded = 0;
    }

    if (isPrintNeeded) {
      if (flags & FLAG_V) {
        putcharV(c);
      } else {
        putchar(c);
      }
    }

    isPrintNeeded = 1;
    isPrevEmptyLine = isEmptyLine;
    prevC = c;
  }
}
