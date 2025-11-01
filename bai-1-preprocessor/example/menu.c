#include <stdio.h>
#include <stdio.h>

#define PRINT_MENU_ITEM(number, item) printf("%d. %s\n", number, item)

#define PRINT_MENU(...)                             \
    do {                                            \
        const char* items[] = {__VA_ARGS__};        \
        int n = sizeof(items) / sizeof(items[0]);   \
        for (int i = 0; i < n; i++) {               \
            PRINT_MENU_ITEM(i + 1, items[i]);       \
        }                                           \
    } while (0)

#define CASE_OPTION(number, function) case number: function(); break;

#define HANDLE_OPTION(option, ...)              \
    switch (option) {                           \
        __VA_ARGS__                             \
        default: printf("Invalid option!\n");   \
    }

void feature1() { printf("Feature 1 selected\n"); }
void feature2() { printf("Feature 2 selected\n"); }
void feature3() { printf("Feature 3 selected\n"); }
void feature4() { printf("Feature 4 selected\n"); }

//tái sử dụng (reusability) : 
// - mã nguồn nên được đóng gói thành 1 hàm để gọi khi cần mà không cần viết lại
// - hệ thống lớn sẽ chứa nhiều module độc lập thực hiện các chức năng khác nhau
// - ví dụ : module màn hình điều khiển chứa các thành phần như : hiển thị menu , xử lý lựa chọn, gọi tính năng 

//dễ dàng mở rộng (Scalability):
// - Đảm bảo không can thiệp mã nguồn khi thêm tính năng mới
// - hệ thống lớn, mã nguồn -> phức tạp, cần tránh sửa trực tiếp mã nguồn 
//  -> tránh lỗi dây chuyền
//  -> đãm bảo tính ổn định cho các phần đã chạy ổn định
int main()
{
    int option;
    PRINT_MENU("option 1","option 2","option 3","option 4","exit");
    printf("nhap lua chon:");
    scanf("%d",&option);
    HANDLE_OPTION(option,
            CASE_OPTION(1,feature1)
            CASE_OPTION(2,feature2)
            CASE_OPTION(3,feature3)
            CASE_OPTION(4,feature4));
    return 0;
}

