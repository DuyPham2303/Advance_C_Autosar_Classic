#include <stdio.h>
#include <stdlib.h> //malloc , calloc, realloc
int arr[5];
int main(){
    // for(int i = 0 ; i < sizeof(arr)/sizeof(arr[0]) ;i++){
    //     arr[i] = i;
    //     printf("arr[%d]:%d\n",i,i);
    // }
    // int a = 21,b = 21,c = 31;

    // arr[7] = 12;
    /* 
        - số lượng phần tử 
        - kiểu dữ liệu mảng
    */
    int size = 0;
    printf("nhập size:");
    scanf("%d",&size);
    int* p = (int*)malloc(size*sizeof(int)); 
    if(p == NULL){
        perror("không đủ vùng nhớ cấp phát trên heap");
        return 1;
    }
    printf("ban đầu\n");
    for(int i = 0 ; i < size ;i++){
        p[i] = i;
        printf("arr[%d]:%d\taddress: %p\n",i,p[i],&p[i]);
    }
    size = size + 5;
    p = (int*)realloc(p,size);
    printf("lúc sau\n");
    for(int i = 0 ; i < size ;i++){
        p[i] = i;
        printf("arr[%d]:%d\taddress: %p\n",i,p[i],&p[i]);
    }

    free(p); 

    p = NULL; //dangling pointer
    

    return 0;
}