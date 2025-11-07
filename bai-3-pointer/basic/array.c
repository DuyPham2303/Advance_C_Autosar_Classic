#include <stdio.h>

/* 
    - định nghĩa , cú pháp * & 
    - cách thức phân bố địa chỉ 
    - endian -> thứ tự lưu trữ byte của dữ liệu (big endian - little endian)
    - kích thước phụ thuộc 
        + kiến trúc hệ điều hành và compiler (PC)
        + kiến trúc bộ vi xử lý  (Vi điều khiển)
    - kiểu dữ liệu quyết định số byte mà nó truy xuất (đọc) trong 1 lần
*/
#include <stdio.h>

void print_array(int *p, int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", *(p + i));
}
int main(){
    int data[3] = {10, 20, 30};


    //gây lỗi, vì tham số truyền vào cần địa chỉ chứ ko phải con trõ
    // int *ptr = data;    
    // print_array(ptr, 3);   

    //cách truyền địa chỉ của hàm chính xác
    print_array(data,3); //truyền tên hàm cũng là địa chỉ 

    return 0;
    int arr[] = {0,1,2,3}; 

    //0 : 0x01,0x02,0x03,0x04
    //1 : 0x05
    //2 : 

    int size = sizeof(arr) / sizeof(arr[0]);

    int* ptr = arr; //&arr 

    //duyệt mảng bằng chỉ số

    // for(int i = 0 ; i < size ; i++){
    //     printf("phần tử thứ %d : %d\tđịa chỉ:0x%p\n",i,ptr[i],&ptr[i]);
    // }

    //duyệt mảng bằng phép toán con trỏ
    /* 
        ptr     : 0x00 (vùng địa chỉ của phần tử thứ 0)
        ptr + 1 : 0x04 (vùng địa chỉ của phần tử thứ 1) 
        ptr + 2 : 0x08 (vùng địa chỉ của phần tử thứ 3) 
        
        + ptr + i.sizeof(data_type) 
        
        => khi cộng/trừ con trỏ, bước nhảy địa chỉ dựa 
        trên kích thước của kiểu dữ liệu con trỏ đang trỏ tới
    */
    for(int i = 0 ; i < size ; i++){
        printf("phần tử thứ %d : %d\tđịa chỉ:0x%p\n",i,*(ptr + i),(ptr + i));
    }



}