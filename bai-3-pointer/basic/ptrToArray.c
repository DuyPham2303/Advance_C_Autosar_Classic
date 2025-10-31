#include <stdio.h>


int main(){
    /* 
        - datatype of array : mỗi phần tử là 4 byte
        - 
    */
    int arr[] = {0,1,2,3}; 
    //0 : 0x00 0x01 0x02 0x03
    //1 : 0x04 0x05 0x06 0x07
    //2 : 0x08 0x09 0x0A 0x0B
    //3 : 0x0C 0x0D 0x0E 0x0F

    int size = sizeof(arr) / sizeof(arr[0]);

    int* ptr = arr; //hoặc &arr

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
        
        => khi cộng/trừ con trỏ, bước nhảy địa chỉ dựa trên kích thước của kiểu dữ liệu mà nó trỏ tới
    */
    for(int i = 0 ; i < size ; i++){
        printf("phần tử thứ %d : %d\tđịa chỉ:0x%p\n",i,*(ptr + i),(ptr + i));
    }



}