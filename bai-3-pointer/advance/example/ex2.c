#include <stdio.h>
#include <string.h>
/* 
    Ví dụ trong hệ thống xe ô tô các ECU sẽ trao đổi dữ liệu với 
    nhau trong qua CAN/LIN/FLEXRAY
    + mỗi ECU có nhiều DTC (Diagnostic Trouble Code - mã chuẩn đoán lỗi)
    + cần cơ chế để lưu trữ bảng mã lỗi của từng ECU, để đễ dàng linh hoạt 
      => chọn ECU nào cần kiểm tra lỗi
      => hiển thị mã lỗi từ ECU đó  
*/
// Giả sử có 3 ECU: Engine, ABS, Airbag, mỗi ECU chứa danh sách quản lý các mã lỗi khác nhau
char* ECU_Engine_Errors[] = {
    "P0100: MAF sensor error",
    "P0200: Injector circuit error",
    NULL
};

char* ECU_ABS_Errors[] = {
    "C0035: Left Front Wheel Speed Sensor",
    "C0040: Right Front Wheel Speed Sensor",
    NULL
};

char* ECU_Airbag_Errors[] = {
    "B0010: Driver Airbag Deployment Loop",
    "B0020: Passenger Airbag Deployment Loop",
    NULL
};

// Mảng con trỏ trỏ tới từng nhóm lỗi (mảng con trỏ khác)
// Đây là lúc double pointer xuất hiện
char** ECU_ErrorTable[] = {
    ECU_Engine_Errors,
    ECU_ABS_Errors,
    ECU_Airbag_Errors
};

// Hàm đọc lỗi từ ECU theo index
void ReadECUErrors(int ecuIndex) {
    printf("== ECU %d Error List ==\n", ecuIndex);
    char** errorList = ECU_ErrorTable[ecuIndex]; 
    int i = 0;
    while (errorList[i] != NULL) {
        printf("Error %d: %s\n", i, errorList[i]);
        i++;
    }
    printf("\n");
}

int main() {
    //đọc lỗi từ từng ECU
    ReadECUErrors(0); // Engine
    ReadECUErrors(1); // ABS
    ReadECUErrors(2); // Airbag

    return 0;
}
