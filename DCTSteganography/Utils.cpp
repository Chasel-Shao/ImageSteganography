#include "Utils.h"
#include <string.h>

void Utils::getFileNameWithoutExtension(const char* file, char* out_buffer) {
    int file_name_len = strlen(file);
    memset(out_buffer, 0, file_name_len + 1);
    strcat(out_buffer, file);
    for (int i = file_name_len - 1; i > -1; --i) {
        char c = out_buffer[i];
        out_buffer[i] = 0x00;
        if (c == '.') {
            break;
        }
    }
}

