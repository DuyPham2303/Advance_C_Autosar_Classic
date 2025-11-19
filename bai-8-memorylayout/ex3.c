#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//cấp phát tĩnh -> số lượng phần tử được cố định
//int arr[1024*1024] = {2, 3, 5, 6, 8};

int main(){
    //mảng = số lượng * kích thước 
    int size = 0;
    
    printf("nhap so phan tu:");
    scanf("%d",&size);

    uint8_t* ptr = (uint8_t*)malloc(size * sizeof(uint8_t)); //0x01,0x02,0x03,0x04,0x05 -> pointer 
    
    if(ptr == NULL){
        printf("không đủ vùng nhớ cấp phát trên heap\n");
        return -1;
    }

    for(int i = 0 ; i < size ; i++){
        ptr[i] = i;
        printf("value[%d]:%d\tadd:%p\n",i,ptr[i],&ptr[i]);
    }

    size += 5;

    ptr = (uint8_t*)realloc(ptr,size*sizeof(uint8_t));

    printf("Sau khi realloc\n");
    for(int i = 0 ; i < size ; i++){
        ptr[i] = i;
        printf("value[%d]:%d\tadd:%p\n",i,ptr[i],&ptr[i]);
    }

    free(ptr);  
    return 0;
}