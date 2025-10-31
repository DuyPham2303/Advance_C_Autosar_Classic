#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======== Prototype ========
void error_unused_var();
void error_no_return();
void error_format_string();
void error_signed_unsigned();
void error_implicit_cast();
void error_extra_argument();
void error_missing_prototype();
void error_unused_function();
void error_unused_result();

// ======== Các lỗi biên dịch / runtime ========
void err_missing_include();
void err_syntax();
void err_semantic();
void err_format_spec();
void err_redefinition();
void err_invalid_extern();
void err_segmentation_fault();
void err_logic_error();
void err_div_by_zero();

int main() {
    int choice;
    printf("=== DEMO CAC LOI PHO BIEN TRONG C ===\n");
    printf("1. Biến khai báo nhưng không dùng\n");
    printf("2. Hàm không trả giá trị\n");
    printf("3. Gọi printf sai định dạng\n");
    printf("4. So sánh signed / unsigned\n");
    printf("5. Gán kiểu ngầm định (implicit cast)\n");
    printf("6. Thừa tham số khi gọi hàm\n");
    printf("7. Thiếu prototype hàm\n");
    printf("8. Biến cục bộ không dùng\n");
    printf("9. Không dùng giá trị trả về (scanf)\n");
    printf("10. Lỗi runtime: segmentation fault\n");
    printf("11. Lỗi chia cho 0\n");
    printf("Chọn lỗi (1-11): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: error_unused_var(); break;
        case 2: error_no_return(); break;
        case 3: error_format_string(); break;
        case 4: error_signed_unsigned(); break;
        case 5: error_implicit_cast(); break;
        case 6: error_extra_argument(); break;
        case 7: error_missing_prototype(); break;
        case 8: error_unused_function(); break;
        case 9: error_unused_result(); break;
        case 10: err_segmentation_fault(); break;
        case 11: err_div_by_zero(); break;
        default: printf("Không hợp lệ!\n"); break;
    }

    return 0;
}

// ====================== CÁC HÀM MÔ PHỎNG ======================

// 1. Biến khai báo nhưng không dùng
void error_unused_var() {
    int a = 10; // -Wall sẽ cảnh báo nếu không dùng
    (void)a;    // bỏ dòng này sẽ có warning
}

// 2. Hàm không trả giá trị dù khai báo int
int error_no_return() {
    int x = 5;
    x++;
    // Không có "return" -> cảnh báo với -Wall
}

// 3. Gọi printf sai định dạng
void error_format_string() {
    int a = 10;
    printf("Gia tri: %f\n", a); // %f nhưng truyền int
}

// 4. So sánh signed / unsigned
void error_signed_unsigned() {
    int a = -1;
    unsigned int b = 2;
    if (a < b) {
        printf("Signed vs unsigned comparison!\n");
    }
}

// 5. Gán kiểu ngầm định
void error_implicit_cast() {
    double d = 3.14;
    int i = d; // cảnh báo -Wconversion
    printf("i = %d\n", i);
}

// 6. Thừa tham số khi gọi hàm
void extra_param(int x) {
    printf("x = %d\n", x);
}
void error_extra_argument() {
    extra_param(5, 10); // Thừa 1 tham số -> cảnh báo
}

// 7. Thiếu prototype (giả định hàm không khai báo trước)
void missing_proto_func(); // tạm tắt prototype để mô phỏng
void error_missing_prototype() {
    missing_proto_func(); // Gọi hàm chưa có prototype thật sự
}

// 8. Biến không dùng (do unused)
void error_unused_function() {
    int x = 100; // cảnh báo unused
    (void)x;     // tạm disable warning
}

// 9. Không dùng giá trị trả về
void error_unused_result() {
    int ret = scanf("%d");
    (void)ret; // nếu bỏ dòng này, gcc -Wunused-result cảnh báo
}

// 10. Segmentation fault (runtime)
void err_segmentation_fault() {
    int *ptr = NULL;
    *ptr = 5; // truy cập NULL pointer
}

// 11. Chia cho 0
void err_div_by_zero() {
    int a = 10, b = 0;
    int c = a / b; // runtime crash
    printf("Kết quả: %d\n", c);
}
