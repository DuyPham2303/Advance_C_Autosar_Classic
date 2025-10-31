#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include "BinarySearch.h"

int BinarySearch(int* pArr,int left,int right,int search){
    if(left <= right){
        int mid = (left + right) / 2;
        if(search == pArr[mid])           return mid;
        else if(search > pArr[mid])      return BinarySearch(pArr,mid + 1,right,search);
        else                        return BinarySearch(pArr,left,mid - 1,search);
    }
   return -1;
}

