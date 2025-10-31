#include "csv_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUF_SIZE 1024
#define CSV_PATH      "../UI/data.csv"

char *getDataPdu(const char *key) {
    FILE *fp = fopen(CSV_PATH, "r");
    if (!fp) {
        return NULL;
    }

    char line[LINE_BUF_SIZE];
    while (fgets(line, sizeof(line), fp)) {
        /* Loại bỏ newline/CR */
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        /* Tách key và phần value tại dấu ',' đầu tiên */
        char *comma = strchr(line, ',');
        if (!comma) {
            continue;
        }
        *comma = '\0';
        char *file_key = line;
        char *file_val = comma + 1;

        if (strcmp(file_key, key) == 0) {
            /* Trả về phần dữ liệu phía sau dấu ',' */
            char *result = (char *)malloc(strlen(file_val) + 1);
            if (!result) {
                fclose(fp);
                return NULL;
            }
            strcpy(result, file_val);
            fclose(fp);
            return result;
        }
    }

    fclose(fp);
    return NULL;
}
