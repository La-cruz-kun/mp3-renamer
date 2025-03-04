#include "parser.c"
#include <locale.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    int i;
    for (i = 0; argv[1][i] != '\0'; i++);
    printf("%s\n", argv[1] + i - 1);
    if (strcmp(argv[1] + i - 4, ".mp3") == 0) {
      read_id3v2(argv[1]);
    } else if (strcmp(argv[1] + i - 1, "/") == 0) {
        int needed_byte = snprintf(NULL, 0, "ls %s", argv[1]);
        char *buffer = malloc(needed_byte);
        if (!buffer) {
          printf("buffer not created\n");
          return 0;
        }

        snprintf(buffer, needed_byte, "ls %s", argv[1]);
        system(buffer);
    } else {
      printf("Invalid address or file\n");
    }
    //printf("Title: %s\n", Title);
    return 0;
}

