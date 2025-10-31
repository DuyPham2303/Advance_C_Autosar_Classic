/* file.h */

#ifndef CSV_LIB_H
#define CSV_LIB_H

#include <stddef.h>   /* For size_t */
#include <stdlib.h>   /* For NULL */
#include <math.h>     /* For NAN */

/**
 * Đọc giá trị kiểu double tương ứng với key trong file CSV.
 * Mỗi dòng trong CSV phải có định dạng "key,value".
 * @param csv_path  Đường dẫn tới file CSV.
 * @param key       Tên khoá cần tìm.
 * @return Giá trị double nếu tìm thấy và parse thành công;
 *         Ngược lại trả về NAN.
 */
double getData(const char *csv_path, const char *key);

/**
 * Cập nhật hoặc thêm mới một key với giá trị double vào file CSV.
 * Nếu key tồn tại, ghi đè value cũ; nếu chưa tồn tại, thêm mới vào cuối.
 * @param csv_path  Đường dẫn tới file CSV.
 * @param key       Tên khoá cần cập nhật/thêm.
 * @param value     Giá trị double cần lưu.
 * @return  0   nếu thành công;
 *         -1   nếu không mở được file;
 *         -2   nếu lỗi cấp phát bộ nhớ;
 *         -3   nếu lỗi ghi file.
 */
int setData(const char *csv_path, const char *key, double value);

/**
 * Đọc giá trị chuỗi (string) tương ứng với key trong file CSV.
 * @param csv_path  Đường dẫn tới file CSV.
 * @param key       Tên khoá cần tìm.
 * @return Một chuỗi malloc’d chứa toàn bộ phần value (caller phải free());
 *         NULL nếu không tìm thấy key hoặc lỗi mở file.
 */
char *getDataCan(const char *csv_path, const char *key);

/**
 * Cập nhật hoặc thêm mới một key với giá trị chuỗi vào file CSV.
 * Nếu key tồn tại, ghi đè value cũ; nếu chưa tồn tại, thêm mới vào cuối.
 * @param csv_path  Đường dẫn tới file CSV.
 * @param key       Tên khoá cần cập nhật/thêm.
 * @param value     Chuỗi giá trị cần lưu.
 * @return  0   nếu thành công;
 *         -1   nếu không mở được file;
 *         -2   nếu lỗi cấp phát bộ nhớ;
 *         -3   nếu lỗi ghi file.
 */
int setDataCan(const char *csv_path, const char *key, const char *value);

#endif /* FILE_H */
