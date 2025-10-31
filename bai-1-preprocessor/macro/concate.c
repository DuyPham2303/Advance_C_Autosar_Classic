#include <stdio.h>

#define DECLARE_VARIABLE(prefix, number) int prefix##number //thay thế câu lệnh khởi tạo biến

int main()
{
    // Sử dụng macro để khai báo các biến động
    DECLARE_VARIABLE(var, 1); // int var1;
    DECLARE_VARIABLE(var, 2); // int var2;
    
    // Gán giá trị cho các biến
    var1 = 10;
    var2 = 20;

    // In ra giá trị của các biến
    printf("var1: %d\n", var1);
    printf("var2: %d\n", var2);
    return 0;
}

