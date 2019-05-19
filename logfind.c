#include <stdio.h>
#include "dbg.h"

#define MAX_LEN 100

int read_file(char **words, int words_len, char *filename)
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
        printf("\tLine contains the word \"%s\": TRUE\n", words[i]);
        debug("AT LEAST ONE MATCH FOUND");
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

  int rc = read_file(argv+1, argc-1, "./log/names.log");
  check(rc >= 0, "FATAL ERROR");

  return 0;

error:
  return 1;
}
