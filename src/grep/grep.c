#include "grep.h"

int main(int argc, char *argv[]) {
  char *pattern = NULL;
  char *filename = NULL;
  char *filePattern = NULL;

  int flags = parseFlags(argc, argv, &pattern, &filename, &filePattern);

  if (pattern == NULL && filePattern == NULL) {
    printf("Ошибка: не задан шаблон\n");
    return 0;
  }

  // берем паттерн из файла
  if (flags & FLAG_FILE_PATTERN) {
    if (filePattern == NULL) {
      printf("Ошибка: не указан файл с регулярными выражениями\n");
      return 0;
    }
    char *filePatternResult = getPatternFromFile(filePattern);
    if (filePatternResult != NULL) {
      pattern = malloc(BUF_SIZE);
      strcpy(pattern, filePatternResult);
      free(filePatternResult);
    }
  }

  int firstFileIndex = optind;

  // подсчет количества файлов и занесение их в массив
  int numFiles = argc - firstFileIndex;
  const char *filenames[numFiles];
  for (int i = 0; i < numFiles; i++) {
    filenames[i] = argv[firstFileIndex + i];
  }

  grepSearch(pattern, filenames, numFiles, flags);

  if (flags & FLAG_FILE_PATTERN) {
    free(pattern);
  }
  return 0;
}

int parseFlags(int argc, char *argv[], char **pattern, char **filename,
               char **filePattern) {
  int opt;
  int flags = FLAG_NONE;

  char *patternOption = NULL;
  char *filePatternOption = NULL;

  // парсим флаги
  while ((opt = getopt(argc, argv, "lne:vichsf:o")) != -1) {
    switch (opt) {
      case 'l':
        flags |= FLAG_SHOW_MATCHING_FILES;
        break;
      case 'n':
        flags |= FLAG_NUMBER_LINES;
        break;
      case 'i':
        flags |= FLAG_IGNORE_CASE;
        break;
      case 'e':
        flags |= FLAG_PATTERN_PROVIDED;
        patternOption = optarg;
        break;
      case 'v':
        flags |= FLAG_INVERT_MATCH;
        break;
      case 'c':
        flags |= FLAG_COUNT_MATCHES;
        break;
      case 'h':
        flags |= FLAG_SHOW_MATCHING_LINES;
        break;
      case 's':
        flags |= FLAG_SUPPRESS_ERRORS;
        break;
      case 'f':
        flags |= FLAG_FILE_PATTERN;
        filePatternOption = optarg;
        break;
      case 'o':
        flags |= FLAG_PRINT_MATCHES;
        break;
      default:
        printf("Ошибка: неверные параметры\n");
        return 0;
    }
  }

  // провер очка на шаблон без флага -e
  if (patternOption == NULL && optind < argc && !(flags & FLAG_FILE_PATTERN)) {
    patternOption = argv[optind];
    optind++;
  }

  // задаем pattern и filename
  if (patternOption != NULL && !(flags & FLAG_FILE_PATTERN)) {
    *pattern = patternOption;
    if (optind < argc) {
      *filename = argv[optind];
    }
  }

  // задаем filePattern
  if (filePatternOption != NULL) {
    *filePattern = filePatternOption;
  }

  return flags;
}

bool searchPattern(const char *pattern, const char *line, bool ignoreCase) {
  bool result;
  regex_t regex;
  int reti;
  int cflags = REG_EXTENDED;
  if (ignoreCase) cflags |= REG_ICASE;

  reti = regcomp(&regex, pattern, cflags);
  if (reti != 0) {
    printf("Ошибка: неверное регулярное выражение\n");
    return 0;
  }

  reti = regexec(&regex, line, 0, NULL, 0);
  regfree(&regex);

  if (reti == 0) {
    result = true;
  } else if (reti == REG_NOMATCH) {
    result = false;
  } else {
    printf("Ошибка: не удалось выполнить регулярное выражение\n");
    return 0;
  }

  return result;
}

void grepSearch(const char *pattern, const char *filenames[], int numFiles,
                int flags) {
  for (int i = 0; i < numFiles; i++) {
    const char *filename = filenames[i];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
      if (!(flags & FLAG_SUPPRESS_ERRORS)) {
        printf("Ошибка: не удалось открыть файл '%s'\n", filename);
      }
      continue;
    }

    char line[BUF_SIZE];
    int lineNumber = 0;
    int matchCount = 0;
    while (fgets(line, sizeof(line), file)) {
      // удаление символа новой строки, если присутствует
      int len = strlen(line);
      if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
      }
      lineNumber++;

      // поиск шаблона в строке
      bool found =
          searchPattern(pattern, line, (flags & FLAG_IGNORE_CASE) != 0);

      // вывод номера строки, если шаблон найден и флаги совпадают
      if ((found && !(flags & FLAG_INVERT_MATCH)) ||
          (!found && (flags & FLAG_INVERT_MATCH))) {
        if (flags & FLAG_NUMBER_LINES) {
          printNumberLines(numFiles, flags, lineNumber, filename);
        }

        if (flags & FLAG_PRINT_MATCHES) {
          printMatches(numFiles, flags, filename, pattern, line);
        } else if (!(flags & FLAG_COUNT_MATCHES) &&
                   !(flags & FLAG_SHOW_MATCHING_FILES)) {
          printLine(numFiles, flags, line, filename);
        }

        matchCount++;
      }
    }

    if (flags & FLAG_COUNT_MATCHES) {
      printMatchCount(numFiles, flags, matchCount, filename);
    }

    if (flags & FLAG_SHOW_MATCHING_FILES && matchCount > 0) {
      printf("%s\n", filename);
    }

    fclose(file);
  }
}

char *getPatternFromFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Ошибка: не удалось открыть файл '%s'\n", filename);
  }

  char *pattern = malloc(BUF_SIZE);
  fgets(pattern, BUF_SIZE, file);
  for (int i = 0; i < BUF_SIZE; i++) {
    if (pattern[i] == '\n') {
      pattern[i] = '\0';
      break;
    }
  }
  fclose(file);
  return pattern;
}

void printMatches(int numFiles, int flags, const char *filename,
                  const char *pattern, const char *line) {
  regmatch_t pmatch;
  regex_t regex;
  int reti;
  int cflags = REG_EXTENDED;  // расширенный синтаксис регулярок
  if (flags & FLAG_IGNORE_CASE) cflags |= REG_ICASE;  // если игнорим регистр

  // компилим регулярку
  reti = regcomp(&regex, pattern, cflags);
  if (reti != 0) {
    printf("Ошибка: неверное регулярное выражение\n");
  }

  // поиск регулярки
  reti = regexec(&regex, line, 1, &pmatch, 0);
  if (reti == 0) {
    printLineOnRegEx(numFiles, flags, pmatch, line, filename);
  }

  regfree(&regex);
}

void printMatchCount(int numFiles, int flags, int matchCount,
                     const char *filename) {
  if (numFiles == 1 || flags & FLAG_SHOW_MATCHING_LINES) {
    printf("%d\n", matchCount);
  } else {
    printf("%s:%d\n", filename, matchCount);
  }
}

void printNumberLines(int numFiles, int flags, int lineNumber,
                      const char *filename) {
  if (numFiles == 1 || flags & FLAG_SHOW_MATCHING_LINES) {
    printf("%d:", lineNumber);
  } else {
    printf("%s:%d:", filename, lineNumber);
  }
}

void printLine(int numFiles, int flags, const char *line,
               const char *filename) {
  if (numFiles == 1 || flags & FLAG_SHOW_MATCHING_LINES ||
      flags & FLAG_NUMBER_LINES) {
    printf("%s\n", line);
  } else {
    printf("%s:%s\n", filename, line);
  }
}

void printLineOnRegEx(int numFiles, int flags, regmatch_t pmatch,
                      const char *line, const char *filename) {
  if (numFiles == 1 || flags & FLAG_SHOW_MATCHING_LINES ||
      flags & FLAG_NUMBER_LINES) {
    printf("%.*s\n", (int)(pmatch.rm_eo - pmatch.rm_so), &line[pmatch.rm_so]);
  } else {
    printf("%s:%.*s\n", filename, (int)(pmatch.rm_eo - pmatch.rm_so),
           &line[pmatch.rm_so]);
  }
}
