#include "BubbleSort.h"
void swap(int* left,int* right){
    int temp = *left;
    *left = *right;
    *right = temp;
}
void bubble_sort(int* pArr,int len){  
    for(int i = 0 ; i < len - 1 ; i++){         //duyệt qua từng phần từ
        for(int j = 0 ; j < len - i - 1; j++){  //duyệt qua các lượt so sánh 
            if(pArr[j] > pArr[j + 1]){
                swap(&pArr[j],&pArr[j+1]);  //hoán đổi vị trí nếu left > right
            }
        }
    }
}