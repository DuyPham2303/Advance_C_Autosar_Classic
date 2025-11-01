#include<stdio.h>
#include<stdarg.h>

// stadarg xử lý các kiểu 
// char, short được mở rộng lên int
// float được mở rộng lên double
//... đại diện cho đối số biến đổi
/* 
    - không biết kiểu
    - không biết số lượng
*/
#include <stdio.h>
#include <stdarg.h>

// Hàm có số lượng đối số biến thiên (variadic function)
void test(int count, ...)
{
    /* note : va_list là con trỏ đến cấu trúc lưu toàn bộ đối số truyền vào hàm */
    //Khai báo biến args để quản lý danh sách đối số biến thiên
    va_list args;

    //Khởi tạo args, trỏ tới đối số đầu tiên sau 'count'
    va_start(args, count);

    /*
        Giả sử ta gọi:
        test(5, 1, 2, "hello", 'a', 3.5);

        => vùng nhớ các đối số sau 'count' lần lượt là:
            [ 1 (int), 2 (int), "hello" (char*), 'a' (int), 3.5 (double) ]

        va_arg(args, type) sẽ:
            - Đọc giá trị hiện tại theo kiểu 'type'
            - Tự động di chuyển con trỏ args tới đối số kế tiếp
    */

    // B3: Lần lượt lấy ra từng đối số với đúng kiểu dữ liệu
    printf("value 1: %d\n", va_arg(args, int));      // => 1
    printf("value 2: %d\n", va_arg(args, int));      // => 2
    printf("value 3: %s\n", va_arg(args, char*));    // => "hello"
    printf("value 4: %c\n", va_arg(args, int));      // => 'a'
    printf("value 5: %f\n", va_arg(args, double));   // => 3.5

    // B4: Kết thúc thao tác, giải phóng trạng thái của va_list
    va_end(args);
}

int main(void)
{
    test(5, 1, 2, "hello", 'a', 3.5);
    return 0;
}
