#include "parser.c"
#include <locale.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

void details()
{
  printf("Artist: %s\n", Artist);
  printf("Album: %s\n", Album);
  printf("Title: %s\n", Title);
}

bool read_dir(char * directory)
{
    DIR *dir = opendir(directory);
    if (dir == NULL) {
      perror("opening of directory failed");
      return false;
    }

    char full_dir[100];
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
      int len;
      for (len = 0; entry->d_name[len] != '\0'; len++);
      if (strcmp(entry->d_name + len - 4, ".mp3") == 0) {
        printf("%s\n", entry->d_name);
        sprintf(full_dir, "%s%s", directory, entry->d_name);
        read_id3v2(full_dir);
        details();
      }
    }
    if (errno != 0) {
      perror("readdir error");
    }
    return true;
}
    

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    int i;
    for (i = 0; argv[1][i] != '\0'; i++);
    printf("%s\n", argv[1] + i - 1);
    if (strcmp(argv[1] + i - 4, ".mp3") == 0) {
      read_id3v2(argv[1]);
      details();
    } else if (strcmp(argv[1] + i - 1, "/") == 0) {
      read_dir(argv[1]);
    } else {
      printf("Invalid address or file\n");
    }
    //printf("Title: %s\n", Title);
    return 0;
}

