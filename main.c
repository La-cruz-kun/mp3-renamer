#include "parser.c"
#include <locale.h>

int main() {
    setlocale(LC_ALL, "");
    read_id3v2("music.mp3");
    printf("Title: %s\n", Title);
    return 0;
}
