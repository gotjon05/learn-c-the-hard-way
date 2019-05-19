#include <stdio.h>
#include "dbg.h"

#define MAX_LEN 100

int read_file(char **words, int words_len, char *filename, char mode)
{
  char buffer[MAX_LEN];
  int count[words_len];
  int num_matches = 0;

  // initialuze the count array
  for (int i = 0; i < words_len; i++) count[i] = 0;

  debug("Number of words to check: %d", words_len);
  debug("Opening file ...");
  FILE *file = fopen(filename, "r");
  check(file, "Failed to open log file!");

  while (fgets(buffer, MAX_LEN, file) != NULL) {
    printf("\n");
    printf("Line in file %s: %s", filename, buffer);
    for (int i = 0; i < words_len; i++) {
      debug("Word %d: %s", i, words[i]);
      if (count[i]) continue;
      if (strstr(buffer, words[i])) {
        printf("Line contains the word \"%s\": TRUE\n", words[i]);
        if (mode == 'o') {
          log_info("AT LEAST ONE MATCH FOUND");
          return 1;
        }
        count[i]++;
        num_matches++;
      }
    }

    // If all matches found, exit the loop
    if (num_matches == words_len) break;
  }

  printf("\n----- RESULTS -----\n");
  log_info("Number of matches: %d", num_matches);
  for (int i = 0; i < words_len; i++) {
    printf("Word: %s, Number of times found: %d\n", words[i], count[i]);
  }
  if(num_matches == words_len) {
    log_info("FOUND ALL WORDS");
    return 1;
  } else return 0;

  fclose(file);
  return 0;

error:
  if (file) fclose(file);
  return -1;
}

int main(int argc, char *argv[])
{
  check(argc > 1, "USAGE: logfind [-o] <args>");
  char *filename = "./log/names.log";
  char **words = argv + 1;
  int word_count = argc - 1;
  char mode = '\0';

  // check if optional argument passed in
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      check(i < 2, "FATAL: Options must come first in list of arguments!");
      switch (argv[i][1]) {
        case 'o':
          log_info("USING OR LOGIC");
          mode = 'o';
          break;
        default:
          sentinel("Invalid Argument!");
      }

      words++;
      word_count--;
    }
  }

  int rc = read_file(words, word_count, filename, mode);
  check(rc >= 0, "FATAL ERROR");

  return 0;

error:
  return 1;
}
