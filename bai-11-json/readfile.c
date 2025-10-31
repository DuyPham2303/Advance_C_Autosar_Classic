#include <stdio.h>
#include <stdlib.h>

/**
 * @brief               hàm nhảy đến vị trí cụ thể được chỉ định trên file
 * @param[in]   f       con trỏ đến file cần thao tác
 * @param[in]   offset  độ dời địa chỉ (tính bằng byte) so với origin
 * @param[in]   origin  vị trí chỉ định trên file tham chiếu so với offset
 *                      - SEEK_SET : vị trí đầu file
 *                      - SEEK_CUR : vị trí hiện tại
 *                      - SEEK_END : vị trí cuối file
 * @return  int
 * @retval  0        thành công
 * @retval  khác 0   lỗi đọc vượt số lượng byte trong file 
 */
int fseek(FILE* f,long offset,int origin);

/**
 * @brief             hàm đọc copy byte từ file vào bộ nhớ, không quan tâm kiểu dữ liệu.
 * @param[in]   dst   con trỏ đến nơi lưu dữ liệu đọc được
 * @param[in]   size  kích thước một phần tử cần đọc (tính bằng byte).
 * @param[in]   n     số phần tử cần đọc (có thể chứa byte rác không thuộc file)
 * @param[in]   f     con trỏ đến file cần thao tác
 * 
 * @return  size_t
 * @retval  số lượng phần tử thực sự được lưu trong file
 */
size_t fread(void * dst, size_t size, size_t n, FILE* f);

char* read_json_file(const char* filename) {
    //mở file ở chế độ đọc 
    FILE *file = fopen(filename, "r");

    char *buffer;   //con trỏ tới chuỗi sẽ đọc ra từ tệp
    long length;    //độ dài của file

    //kiểm tra file hợp lệ
    if (!file) {
        perror("Không thể mở file");
        return NULL;
    }

    // Di chuyển tới cuối file để lấy kích thước
    fseek(file, 0, SEEK_END);

    //trả về độ dài của tệp
    length = ftell(file);
    fseek(file, 0, SEEK_SET); // quay lại đầu file

    buffer = malloc(length + 1); // cấp phát động để lưu chuỗi đọc về từ tệp
    if (!buffer) {
        perror("Không đủ bộ nhớ");
        fclose(file);
        return NULL;
    }    
   
    fread(buffer,1, length, file);
    buffer[length] = '\0'; // Kết thúc chuỗi

    //thao tác đóng file an toàn 
    fclose(file);
    return buffer;
}
