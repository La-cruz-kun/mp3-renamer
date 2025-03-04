#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#define HEADER_SIZE 10

unsigned int syncsafe_to_int(unsigned char *bytes) {
    return (bytes[0] << 21) | (bytes[1] << 14) | (bytes[2] << 7) | bytes[3];
}

void read_frame(FILE *file, char *frame_id, unsigned int tag_size) {
    unsigned int frame_start = ftell(file);
    while (ftell(file) < tag_size + HEADER_SIZE) {
      char frame_header[10];
      fread(frame_header, 1, 10, file);

      if (strncmp(frame_header, frame_id, 4) == 0) {
          unsigned int frame_size = syncsafe_to_int((unsigned char *)frame_header + 4);
          printf("%s: ", frame_id);

          unsigned char *data = malloc(frame_size + 1);
          fread(data, 1, frame_size, file);
          data[frame_size] = '\0';

          // Skip encoding byte
          if (data[0] == 0x03) {
            printf("%s\n", data + 1);
          }else if (data[0] == 0x01) {
            wchar_t * wdata = (wchar_t *) data + 1;
            wprintf(L"%ls\n", wdata);
          } else {
            printf("%s\n", data + 1);
          }

          free(data);
      } else {
          // Seek to next frame
          unsigned int frame_size = syncsafe_to_int((unsigned char *)frame_header + 4);
          fseek(file, frame_size, SEEK_CUR);
      }
    }
    fseek(file, frame_start, SEEK_SET);
}

void read_id3v2(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("File open error");
        return;
    }

    setlocale(LC_ALL, "");
    char header[HEADER_SIZE];
    fread(header, 1, HEADER_SIZE, file);

    if (strncmp(header, "ID3", 3) != 0) {
        printf("No ID3v2 metadata found\n");
        fclose(file);
        return;
    }

    unsigned int tag_size = syncsafe_to_int((unsigned char *)header + 6);
    printf("Tag Size: %u bytes\n", tag_size);

    read_frame(file, "TIT2", tag_size);  // Title
    read_frame(file, "TALB", tag_size);
    read_frame(file, "TPE1", tag_size);  // Artist

    fclose(file);
}

int main() {
    read_id3v2("music4.mp3");
    return 0;
}
