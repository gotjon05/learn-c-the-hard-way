#include <stdio.h>

void crash() {
  char *test = NULL;

  for(int i = 0; i < 100000; i++) {
    printf("%c", test[i]);
  }
}

int main()
{
  int age = 10;
  int height = 72;

  printf("I am %d years old.\n", age);
  printf("I am %d inches tall.\n", height);

  crash();

  return 0;
}
