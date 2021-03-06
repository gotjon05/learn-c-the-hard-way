#include <stdio.h>
#include <glob.h>
#include "dbg.h"

#define MAX_LEN 100

char **get_file_list()
{
  FILE *file;
  char buffer[MAX_LEN];
  glob_t result;

  file = fopen("./.logfind", "r");
  check(file, "Cannot locate .logfind");

  // Get the list of allowed file types
  log_info("Reading log files ...");
  log_info("-----");
  while (fgets(buffer, MAX_LEN, file) != NULL) {
    // allow comments and line breaks
    if (buffer[0] == '#' || buffer[0] == '\n') continue;

    // Chomp newline off the buffer string
    int len = strlen(buffer);
    if (buffer[len-1] == '\n') buffer[len-1] = 0;

    // Get the list of files
    int rc = glob(buffer, GLOB_ERR && GLOB_APPEND, NULL, &result);
    check(rc != GLOB_NOMATCH, "No log files found: %s", buffer);
  }

  int i = 0;
  while (result.gl_pathv[i]) {
    log_info("%s", result.gl_pathv[i]);
    i++;
  }

  // Return the list of files
  fclose(file);
  return result.gl_pathv;

error:
  if (file) fclose(file);
  return NULL;
}

int read_file(char **words, int words_len, char *filename, char mode)
{
  char buffer[MAX_LEN];
  int count[words_len];
  int num_matches = 0;

  // initialuze the count array
  for (int i = 0; i < words_len; i++) count[i] = 0;

  log_info("-----");
  log_info("Opening file \"%s\" ...", filename);
  FILE *file = fopen(filename, "r");
  check(file, "Failed to open log file!");

  debug("Number of words to check: %d", words_len);
  log_info("-----");
  while (fgets(buffer, MAX_LEN, file) != NULL) {
    // printf("\n");
    // printf("Line in file %s: %s", filename, buffer);
    for (int i = 0; i < words_len; i++) {
      // debug("Word %d: %s", i, words[i]);
      if (count[i]) continue;
      if (strstr(buffer, words[i])) {
        // printf("Line contains the word \"%s\": TRUE\n", words[i]);
        if (mode == 'o') {
          log_info("%s: " KYEL "MATCH FOUND:" KNRM " %s", filename, words[i]);

          fclose(file);
          return 0;
        }
        count[i]++;
        num_matches++;
      }
    }

    // If all matches found, exit the loop
    if (num_matches == words_len) break;
  }

  // printf("\n----- RESULTS -----\n");
  // log_info("Number of matches: %d", num_matches);
  // for (int i = 0; i < words_len; i++) {
  //   printf("Word: %s, Number of times found: %d\n", words[i], count[i]);
  // }
  if(num_matches == words_len) {
    log_info("%s: " KGRN "ALL MATCHES FOUND" KNRM, filename);
  } else {
    log_info("%s: " KRED "NO MATCHES FOUND" KNRM, filename);
  }

  fclose(file);
  return 0;

error:
  if (file) fclose(file);
  return -1;
}

int main(int argc, char *argv[])
{
  check(argc > 1, "USAGE: logfind [-o] [words]");
  char **filenames = get_file_list();
  check(filenames != NULL, KRED "FATAL: " KNRM "Could not read logfiles.");
  char **words = argv + 1;
  int word_count = argc - 1;
  char mode = '\0';
  int rc = -1;

  // check if optional argument passed in
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      check(i < 2 && argv[i][2] == '\0', "FATAL: Invalid Option Format!");
      switch (argv[i][1]) {
        case 'o':
          debug("USING OR LOGIC");
          mode = 'o';
          break;
        default:
          sentinel("Invalid Option!");
      }

      words++;
      word_count--;
    }
  }

  for (int i = 0; filenames[i] != NULL; i++) {
    rc = read_file(words, word_count, filenames[i], mode);
    check(rc == 0, "FATAL ERROR");
  }

  return 0;

error:
  return 1;
}
