#include <wchar.h>
#include <stdio.h>
#include <iconv.h>
#include <locale.h>

void print_text(unsigned char *data, int size) {
    unsigned char encoding = data[0];

    if (encoding == 0x01) {  // UTF-16 with BOM
      iconv_t cd = iconv_open("UTF-8", "UTF-16LE");
      size_t in_bytes = size;
      printf("%zu\n", in_bytes);
      char *in_buf = (char *)&data[3];
      char out_buf[100];
      char *out_ptr = out_buf;
      size_t out_bytes = sizeof(out_buf);

      iconv(cd, &in_buf, &in_bytes, &out_ptr, &out_bytes);
      iconv_close(cd);
      printf("%s\n", out_buf);
    }
    else if (encoding == 0x03) {  // UTF-8
        printf("%s\n", data + 1);
    }
    else {
        printf("Unknown Encoding\n");
    }
}

int main() {
    setlocale(LC_ALL, "");  // Use UTF-8 Locale
    unsigned char japanese[] = {0x01, 0xFF, 0xFE, 0xB5, 0x30, 0xF3, 0x30, 0xD7, 0x30, 0xEB, 0x30, '\0'};  // "サンプル" (Sample)
    

    printf("Japanese Title: ");
    print_text(japanese, sizeof(japanese));

    return 0;
}
