/********************************************************
 *******************
 * @file FileOperation.h
 * @brief Khai báo các hàm để truy cập / lưu trữ dữ liệu từ database
 * @details File này cung cấp định nghĩa các hàm Load_UserDB,Print_UserDB,Remove_UserDB, 
 *          Header này chỉ nên được include bởi module App
 * @version 1.0
 * @date 2025-16-8
 * @author Pham Cao Duy
 *********************************************************
 ******************/
#ifndef ManageDB_H
#define ManageDB_H
#include "UserData.h"

/**
 * @brief   Hàm đọc dữ liệu từ file csv và lưu trũ váo cấu trúc UserData
 * @retval  E_OK    Hàm xử lý thành công
 * @retval  NOT_OK  Hàm xử lý thất bại
 */
Status Load_UserDB(const char* filename);
/**
 * @brief   Hàm xóa dữ liệu của user đã lưu trữ
 * @retval  E_OK    Hàm xử lý thành công
 * @retval  NOT_OK  Hàm xử lý thất bại
 */
Status Remove_UserDB();
/**
 * @brief   Hàm in ra thông tin của toàn bộ user đọc được từ file csv
 * @retval  none
 */
void Print_UserDB();
#endif
