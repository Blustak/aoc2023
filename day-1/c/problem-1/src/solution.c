#include <stdio.h>
#include <stdlib.h>
#define BUFFER_LENGTH 100

int main(int argc, char **argv) {
  char *file_path = argv[1];
  FILE *fptr;
  fptr = fopen(file_path, "r");
  char *buffer = malloc(sizeof(char) * BUFFER_LENGTH);
  long sum = 0;

  while (fgets(buffer, sizeof(buffer), fptr)) {
    int first;
    int last;
    // front search
    for (int i = 0; i < sizeof(buffer); i++) {
      if (buffer[i] < '1' || buffer[i] > '9') {
        continue;
      }

      else {
        first = buffer[i];
        break;
      }
    }
  }

  free(buffer);

  printf("Hello, world!\n");
  return 0;
}
