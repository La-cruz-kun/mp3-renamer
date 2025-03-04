#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

void print_text(unsigned char *data, int size) {
    setlocale(LC_ALL, "");  // Use UTF-8 Locale
    unsigned char encoding = data[0];

    if (encoding == 0x01) {  // UTF-16 with BOM
        wchar_t *wstr = (wchar_t *)(data + 1);

        // Strip BOM if present
        if (wstr[0] == 0xFEFF || wstr[0] == 0xFFFE) {
            wstr++;
        }
        wprintf(L"%ls\n", wstr);  // Print Unicode Text
    }
    else if (encoding == 0x03) {  // UTF-8
        wprintf(L"%s\n", data + 1);
    }
    else {
        wprintf(L"Unknown Encoding\n");
    }
}

int main() {
    unsigned char japanese[] = {0x01, 0xFF, 0xFE, 0xCF, 0x30, 0x6B, 0x30, 0x4F, 0x30, 0};  // "サンプル" (Sample)
    
    wprintf(L"Japanese Title: ");
    print_text(japanese, sizeof(japanese));

    return 0;
}
