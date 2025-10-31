/* file.c */

#include "csv_lib.h"
#include <stdio.h>     /* fopen, fgets, fprintf, fclose */
#include <stdlib.h>    /* malloc, free, realloc, strtod, NULL */
#include <string.h>    /* strlen, strchr, strcmp, strdup, strncpy */
#include <ctype.h>     /* isspace */
#include <errno.h>     /* errno, ERANGE */

/* ============================ */
/* Struct lưu trữ cặp key/value */
/* ============================ */
typedef struct {
    char *key;
    char *value;
} Entry;

/* ========================================================= */
/* Helper: trim_whitespace: loại bỏ khoảng trắng đầu/cuối    */
/* @param s Chuỗi gốc. Trả về con trỏ tới ký tự đầu tiên không phải whitespace. */
/* ========================================================= */
static char *trim_whitespace(char *s) {
    char *start = s;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    if (*start == '\0') {
        return start;
    }
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    return start;
}

/* ========================================================= */
/* load_entries: đọc toàn bộ file CSV vào mảng Entry[]       */
/* @param path         Đường dẫn file CSV                     */
/* @param out_entries  Địa chỉ con trỏ Entry*, sẽ trỏ ra mảng  */
/* @param out_count    Nơi lưu số phần tử trong mảng            */
/* @return 0 nếu thành công; <0 nếu lỗi (mở file, cấp phát)    */
/* ========================================================= */
static int load_entries(const char *path, Entry **out_entries, size_t *out_count) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        return -1; /* Lỗi mở file */
    }

    Entry *entries = NULL;
    size_t count = 0, capacity = 0;
    char line[512];

    while (fgets(line, sizeof(line), fp)) {
        /* Loại bỏ newline (\n hoặc \r\n) */
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
            if (len > 1 && line[len - 2] == '\r') {
                line[len - 2] = '\0';
            }
        }

        /* Tìm dấu phẩy đầu tiên */
        char *comma = strchr(line, ',');
        if (!comma) {
            continue; /* Bỏ qua dòng không hợp lệ */
        }

        *comma = '\0';
        char *keyPart   = trim_whitespace(line);
        char *valuePart = trim_whitespace(comma + 1);

        char *keyDup = strdup(keyPart);
        char *valDup = strdup(valuePart);
        if (!keyDup || !valDup) {
            free(keyDup);
            free(valDup);
            for (size_t i = 0; i < count; i++) {
                free(entries[i].key);
                free(entries[i].value);
            }
            free(entries);
            fclose(fp);
            return -2; /* Lỗi cấp phát */
        }

        if (count >= capacity) {
            size_t newCap = (capacity == 0) ? 8 : capacity * 2;
            Entry *tmp = realloc(entries, newCap * sizeof(Entry));
            if (!tmp) {
                free(keyDup);
                free(valDup);
                for (size_t i = 0; i < count; i++) {
                    free(entries[i].key);
                    free(entries[i].value);
                }
                free(entries);
                fclose(fp);
                return -2;
            }
            entries = tmp;
            capacity = newCap;
        }

        entries[count].key   = keyDup;
        entries[count].value = valDup;
        count++;
    }

    fclose(fp);
    *out_entries = entries;
    *out_count   = count;
    return 0;
}

/* ========================================================= */
/* write_entries: ghi ngược mảng Entry[] về file CSV         */
/* @param path     Đường dẫn file CSV                         */
/* @param entries  Mảng entries                                */
/* @param count    Số phần tử                                   */
/* @return 0 nếu thành công; <0 nếu lỗi mở file                */
/* ========================================================= */
static int write_entries(const char *path, Entry *entries, size_t count) {
    FILE *fp = fopen(path, "w");
    if (!fp) {
        return -1; /* Lỗi mở file để ghi */
    }
    for (size_t i = 0; i < count; i++) {
        fprintf(fp, "%s,%s\n", entries[i].key, entries[i].value);
    }
    fclose(fp);
    return 0;
}

/* ========================================================= */
/* getData: đọc giá trị kiểu double cho key từ file CSV      */
/* @param csv_path  Đường dẫn file CSV                        */
/* @param key       Key cần tìm                                */
/* @return Giá trị double nếu tìm thấy & parse thành công;    */
/*         Ngược lại trả về NAN.                               */
/* ========================================================= */
double getData(const char *csv_path, const char *key) {
    if (!csv_path || !key) {
        return NAN;
    }

    Entry *entries = NULL;
    size_t count = 0;
    int ret = load_entries(csv_path, &entries, &count);
    if (ret < 0) {
        return NAN;
    }

    double result = NAN;
    for (size_t i = 0; i < count; i++) {
        if (strcmp(entries[i].key, key) == 0) {
            errno = 0;
            char *endptr;
            double val = strtod(entries[i].value, &endptr);
            if (endptr != entries[i].value && errno != ERANGE) {
                result = val;
            } else {
                result = NAN;
            }
            break;
        }
    }

    for (size_t i = 0; i < count; i++) {
        free(entries[i].key);
        free(entries[i].value);
    }
    free(entries);

    return result;
}

/* ========================================================= */
/* setData: cập nhật hoặc thêm mới key với giá trị double    */
/* @param csv_path  Đường dẫn file CSV                         */
/* @param key       Key cần cập nhật/thêm                        */
/* @param value     Giá trị double cần lưu                      */
/* @return  0 nếu thành công;                                  */
/*         -1 nếu lỗi mở file;                                */
/*         -2 nếu lỗi cấp phát bộ nhớ;                        */
/*         -3 nếu lỗi ghi file                                 */
/* ========================================================= */
int setData(const char *csv_path, const char *key, double value) {
    if (!csv_path || !key) {
        return -1;
    }

    Entry *entries = NULL;
    size_t count = 0;
    int ret = load_entries(csv_path, &entries, &count);
    if (ret < 0) {
        return ret;
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "%g", value);

    int found = 0;
    for (size_t i = 0; i < count; i++) {
        if (strcmp(entries[i].key, key) == 0) {
            free(entries[i].value);
            entries[i].value = strdup(buf);
            if (!entries[i].value) {
                for (size_t j = 0; j < count; j++) {
                    free(entries[j].key);
                    free(entries[j].value);
                }
                free(entries);
                return -2;
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        Entry *tmp = realloc(entries, (count + 1) * sizeof(Entry));
        if (!tmp) {
            for (size_t i = 0; i < count; i++) {
                free(entries[i].key);
                free(entries[i].value);
            }
            free(entries);
            return -2;
        }
        entries = tmp;
        entries[count].key   = strdup(key);
        entries[count].value = strdup(buf);
        if (!entries[count].key || !entries[count].value) {
            free(entries[count].key);
            free(entries[count].value);
            for (size_t i = 0; i < count; i++) {
                free(entries[i].key);
                free(entries[i].value);
            }
            free(entries);
            return -2;
        }
        count++;
    }

    ret = write_entries(csv_path, entries, count);
    if (ret < 0) {
        for (size_t i = 0; i < count; i++) {
            free(entries[i].key);
            free(entries[i].value);
        }
        free(entries);
        return -3;
    }

    for (size_t i = 0; i < count; i++) {
        free(entries[i].key);
        free(entries[i].value);
    }
    free(entries);

    return 0;
}

/* ========================================================= */
/* getDataCan: đọc giá trị chuỗi (string) cho key             */
/* @param csv_path  Đường dẫn file CSV                         */
/* @param key       Key cần tìm                                */
/* @return Một chuỗi malloc’d chứa toàn bộ phần value;         */
/*         Caller phải free().                                */
/*         NULL nếu không tìm thấy key hoặc lỗi mở file.      */
/* ========================================================= */
char *getDataCan(const char *csv_path, const char *key) {
    if (!csv_path || !key) {
        return NULL;
    }

    Entry *entries = NULL;
    size_t count = 0;
    int ret = load_entries(csv_path, &entries, &count);
    if (ret < 0) {
        return NULL;
    }

    char *result = NULL;
    for (size_t i = 0; i < count; i++) {
        if (strcmp(entries[i].key, key) == 0) {
            result = strdup(entries[i].value);
            break;
        }
    }

    for (size_t i = 0; i < count; i++) {
        free(entries[i].key);
        free(entries[i].value);
    }
    free(entries);

    return result;
}

/* ========================================================= */
/* setDataCan: cập nhật hoặc thêm mới key với giá trị chuỗi   */
/* @param csv_path  Đường dẫn file CSV                         */
/* @param key       Key cần cập nhật/thêm                        */
/* @param value     Chuỗi giá trị cần lưu                       */
/* @return  0 nếu thành công;                                  */
/*         -1 nếu lỗi mở file;                                */
/*         -2 nếu lỗi cấp phát bộ nhớ;                        */
/*         -3 nếu lỗi ghi file                                 */
/* ========================================================= */
int setDataCan(const char *csv_path, const char *key, const char *value) {
    if (!csv_path || !key || !value) {
        return -1;
    }

    Entry *entries = NULL;
    size_t count = 0;
    int ret = load_entries(csv_path, &entries, &count);
    if (ret < 0) {
        return ret;
    }

    int found = 0;
    for (size_t i = 0; i < count; i++) {
        if (strcmp(entries[i].key, key) == 0) {
            free(entries[i].value);
            entries[i].value = strdup(value);
            if (!entries[i].value) {
                for (size_t j = 0; j < count; j++) {
                    free(entries[j].key);
                    free(entries[j].value);
                }
                free(entries);
                return -2;
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        Entry *tmp = realloc(entries, (count + 1) * sizeof(Entry));
        if (!tmp) {
            for (size_t i = 0; i < count; i++) {
                free(entries[i].key);
                free(entries[i].value);
            }
            free(entries);
            return -2;
        }
        entries = tmp;
        entries[count].key   = strdup(key);
        entries[count].value = strdup(value);
        if (!entries[count].key || !entries[count].value) {
            free(entries[count].key);
            free(entries[count].value);
            for (size_t i = 0; i < count; i++) {
                free(entries[i].key);
                free(entries[i].value);
            }
            free(entries);
            return -2;
        }
        count++;
    }

    ret = write_entries(csv_path, entries, count);
    if (ret < 0) {
        for (size_t i = 0; i < count; i++) {
            free(entries[i].key);
            free(entries[i].value);
        }
        free(entries);
        return -3;
    }

    for (size_t i = 0; i < count; i++) {
        free(entries[i].key);
        free(entries[i].value);
    }
    free(entries);

    return 0;
}
