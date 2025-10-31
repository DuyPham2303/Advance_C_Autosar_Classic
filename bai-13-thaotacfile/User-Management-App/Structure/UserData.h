/********************************************************
 *******************
 * @file ManageUser.h
 * @brief Khai báo cấu trúc lưu dữ liệu User 
 * @details File này cung cấp định nghĩa cấu trúc chứa các trường thông tin của user,
 *          Header này chỉ nên được include bởi module Service,UserNode,CenterPoint
 * @version 1.0
 * @date 2025-16-8
 * @author Pham Cao Duy
 *********************************************************
 ******************/
#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include "StdTypes.h"

/*
 * Định nghĩa cấu trúc UserData chứa thông tin về đối tượng User
 * + name: tên của đối tượng
 * + address: địa chỉ 
 * + phone: số điện thoại
 * + age : tuổi
 */
typedef struct{
    char* name;
    char* address;
    char* phone;
    uint8_t age;
}UserData;

#endif