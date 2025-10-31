/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>     /* For isnan */
#include "csv_lib.h"     /* Đảm bảo file.h nằm cùng thư mục hoặc trong include path */

/*
 * Trước khi chạy chương trình này, hãy tạo sẵn một file "data.csv" trong cùng thư mục,
 * với nội dung ví dụ như sau:
 *
 * temp,28.81
 * voltage,51.22
 * current,17.57
 * torque,65.00
 * currentSpeed,55.00
 * requestedSpeed,120
 * can,0x3F0 8 04 00 00 00 00 00 00 00
 *
 * Bạn có thể điều chỉnh hoặc thêm bớt các dòng tuỳ ý.
 */

int main(void) {
    const char *csvFile = "data.csv";
    double val;
    char *str;

    printf("=== TEST getData (double) ===\n");
    val = getData(csvFile, "temp");
    if (!isnan(val)) {
        printf("  getData(\"temp\") = %.2f\n", val);
    } else {
        printf("  getData(\"temp\"): Không tìm thấy key hoặc parse lỗi\n");
    }

    val = getData(csvFile, "voltage");
    if (!isnan(val)) {
        printf("  getData(\"voltage\") = %.2f\n", val);
    } else {
        printf("  getData(\"voltage\"): Không tìm thấy key hoặc parse lỗi\n");
    }

    val = getData(csvFile, "nonExistingKey");
    if (!isnan(val)) {
        printf("  getData(\"nonExistingKey\") = %.2f\n", val);
    } else {
        printf("  getData(\"nonExistingKey\"): Không tìm thấy key hoặc parse lỗi\n");
    }

    val = getData(csvFile, "can");
    if (!isnan(val)) {
        printf("  getData(\"can\") = %.2f (nhưng giá trị gốc không phải double)\n", val);
    } else {
        printf("  getData(\"can\"): parse lỗi vì value không phải số\n");
    }

    printf("\n=== TEST setData (cập nhật/ thêm mới double) ===\n");
    if (setData(csvFile, "temp", 30.55) == 0) {
        printf("  setData(\"temp\", 30.55): Thành công.\n");
        /* Đọc lại để xác nhận */
        val = getData(csvFile, "temp");
        if (!isnan(val)) {
            printf("    → Sau khi cập nhật, getData(\"temp\") = %.2f\n", val);
        }
    } else {
        printf("  setData(\"temp\", 30.55): Thất bại\n");
    }

    if (setData(csvFile, "newTemperature", 45.99) == 0) {
        printf("  setData(\"newTemperature\", 45.99): Thêm mới thành công.\n");
        val = getData(csvFile, "newTemperature");
        if (!isnan(val)) {
            printf("    → getData(\"newTemperature\") = %.2f\n", val);
        }
    } else {
        printf("  setData(\"newTemperature\", 45.99): Thất bại\n");
    }

    printf("\n=== TEST getDataCan (string) ===\n");
    str = getDataCan(csvFile, "can");
    if (str) {
        printf("  getDataCan(\"can\") = \"%s\"\n", str);
        free(str);
    } else {
        printf("  getDataCan(\"can\"): Không tìm thấy key hoặc lỗi\n");
    }

    str = getDataCan(csvFile, "nonExistingKey");
    if (str) {
        printf("  getDataCan(\"nonExistingKey\") = \"%s\"\n", str);
        free(str);
    } else {
        printf("  getDataCan(\"nonExistingKey\"): Không tìm thấy key hoặc lỗi\n");
    }

    printf("\n=== TEST setDataCan (cập nhật/ thêm mới string) ===\n");
    const char *newCanValue = "0x3F0 8 0A 11 22 33 44 55 66 77";
    if (setDataCan(csvFile, "can", newCanValue) == 0) {
        printf("  setDataCan(\"can\", \"%s\"): Cập nhật thành công.\n", newCanValue);
        str = getDataCan(csvFile, "can");
        if (str) {
            printf("    → Sau khi cập nhật, getDataCan(\"can\") = \"%s\"\n", str);
            free(str);
        }
    } else {
        printf("  setDataCan(\"can\", \"%s\"): Thất bại\n", newCanValue);
    }

    if (setDataCan(csvFile, "newStatus", "OK") == 0) {
        printf("  setDataCan(\"newStatus\", \"OK\"): Thêm mới thành công.\n");
        str = getDataCan(csvFile, "newStatus");
        if (str) {
            printf("    → getDataCan(\"newStatus\") = \"%s\"\n", str);
            free(str);
        }
    } else {
        printf("  setDataCan(\"newStatus\", \"OK\"): Thất bại\n");
    }

    printf("\n=== KẾT THÚC TEST ===\n");
    return 0;
}
