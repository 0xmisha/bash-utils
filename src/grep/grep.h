#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 65536

// Флаги с помощью enum и побитовых сдвигов
enum Flags {
  FLAG_NONE = 0,
  FLAG_IGNORE_CASE = 1 << 0,
  FLAG_PATTERN_PROVIDED = 1 << 1,
  FLAG_INVERT_MATCH = 1 << 2,
  FLAG_COUNT_MATCHES = 1 << 3,
  FLAG_SHOW_MATCHING_FILES = 1 << 4,
  FLAG_NUMBER_LINES = 1 << 5,
  FLAG_SUPPRESS_ERRORS = 1 << 6,
  FLAG_SHOW_MATCHING_LINES = 1 << 7,
  FLAG_FILE_PATTERN = 1 << 8,
  FLAG_PRINT_MATCHES = 1 << 9
};

// Функция для парсинга флагов с помощью getopt
int parseFlags(int argc, char *argv[], char **pattern, char **filename,
               char **filePattern);

// Функция поиска шаблона в строке
bool searchPattern(const char *pattern, const char *line, bool ignoreCase);

// Функция поиска шаблона в файле и вывод соответствующих строк
void grepSearch(const char *pattern, const char *filenames[], int numFiles,
                int flags);

// Функция для получения регулярных выражений из файла
char *getPatternFromFile(const char *filename);

// Вспомогательные функции под флаги

void printNumberLines(int numFiles, int flags, int lineNumber,
                      const char *filename);

void printMatchCount(int numFiles, int flags, int matchCount,
                     const char *filename);

void printLine(int numFiles, int flags, const char *line, const char *filename);

void printLineOnRegEx(int numFiles, int flags, regmatch_t pmatch,
                      const char *line, const char *filename);

void printMatches(int numFiles, int flags, const char *filename,
                  const char *pattern, const char *line);
