#include <stdio.h>

int main(int argc, char *argvp[])
{
  int i = 0;
  while (i < 25) {
    printf("%d", i);
    i++;
  }

  // need this to add a final newline
  printf("\n");

  return 0;
}
