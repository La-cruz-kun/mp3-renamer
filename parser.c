#include <ctype.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 10
#define SIZE 100

unsigned char Title[SIZE];
unsigned char Album[SIZE];
unsigned char Artist[SIZE];
unsigned char Value[SIZE] = {0};
unsigned char TEMP[SIZE] = {0};

unsigned int syncsafe_to_int(unsigned char *bytes) {
    return (bytes[0] << 21) | (bytes[1] << 14) | (bytes[2] << 7) | bytes[3];
}

void sanitize_filename(char * filename)
{
  char *src = filename;
  char *dst = filename;
  while (*src) {
    if (*src == '/' || *src == '\0') {
      src++;
      continue;
    }
    *dst++ = *src++;
  }
  *dst = '\0';
}

void r_file()
{
    strncpy((char *)TEMP, (char *)Value, sizeof(TEMP));
    TEMP[sizeof(TEMP) - 1] = '\0';
    sanitize_filename((char *)TEMP);
    if (strlen((char *)TEMP) == 0) {
      printf("%s\n", Title);
      fprintf(stderr, "filename invalid after sanitization\n");
      return;
    }
}

void read_frame(FILE *file, char *frame_id, unsigned int tag_size) {
    unsigned int frame_start = ftell(file);
    while (ftell(file) < tag_size + HEADER_SIZE) {
      char frame_header[10];
      fread(frame_header, 1, 10, file);

      if (strncmp(frame_header, frame_id, 4) == 0) {
          unsigned int frame_size = syncsafe_to_int((unsigned char *)frame_header + 4);

          unsigned char *data = malloc(frame_size + 1);
          fread(data, 1, frame_size, file);
          data[frame_size] = '\0';

          // Skip encoding byte
          if (data[0] == 0x03) {
            memcpy(Value, data + 1, (frame_size + 1));
            fseek(file, frame_start, SEEK_SET);
            return;
          }else if (data[0] == 0x01) {
            // use iconv header to parser utf 16
            iconv_t cd = iconv_open("UTF-8", "UTF-16LE"); // define the conversion
            size_t in_bytes = frame_size; // the size of the input
            char *in_buf = (char *)&data[3]; // the input itself starts at index 3 because we skip both the encoding info and the BOM
            char out_buf[100];
            char *out_ptr = out_buf;
            size_t out_bytes = sizeof(out_buf);

            iconv(cd, &in_buf, &in_bytes, &out_ptr, &out_bytes); // do the conversion
            iconv_close(cd);
            memcpy(Value, out_buf, out_bytes);
            fseek(file, frame_start, SEEK_SET);
            return;
          } else {
            memcpy(Value, data + 1, (frame_size + 1));
            fseek(file, frame_start, SEEK_SET);
            return;
          }

          free(data);
      } else {
          // Seek to next frame
          unsigned int frame_size = syncsafe_to_int((unsigned char *)frame_header + 4);
          fseek(file, frame_size, SEEK_CUR);
      }
    }
    fseek(file, frame_start, SEEK_SET);
    Value[0] = '\0';
}

void read_id3v2(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("File open error");
        return;
    }

    char header[HEADER_SIZE];
    fread(header, 1, HEADER_SIZE, file);

    if (strncmp(header, "ID3", 3) != 0) {
        printf("No ID3v2 metadata found\n");
        fclose(file);
        return;
    }

    unsigned int tag_size = syncsafe_to_int((unsigned char *)header + 6);

    read_frame(file, "TIT2", tag_size);  // Title
    r_file();
    memcpy(Title, TEMP, sizeof(TEMP));
    read_frame(file, "TALB", tag_size);
    r_file();
    memcpy(Album, TEMP, sizeof(TEMP));
    read_frame(file, "TPE1", tag_size);  // Artist
    r_file();
    memcpy(Artist, TEMP, sizeof(TEMP));

    fclose(file);
}

