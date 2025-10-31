#include <stdio.h>
#include "LinearSearch.h"

int linear_search(const int* arr, int size, int search){
    for (int i = 0; i < size; i++) {
        if (arr[i] == search) {
            return i; // Trả về vị trí tìm thấy
        }
    }
    return -1; // Không tìm thấy
}

