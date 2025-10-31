/********************************************************
 *******************
 * @file BuildTree.h
 * @brief Khai báo các hàm để triển khai thuật toán tìm kiếm thông tin user
 * @details File này cung cấp định nghĩa các hàm Build_SearchList,Search_User,Remove_SearchList 
 *          Header này chỉ nên được include bởi module App
 * @version 1.0
 * @date 2025-16-8
 * @author Pham Cao Duy
 *********************************************************
 ******************/
#ifndef BUILDTREE_H
#define BUILDTREE_H
#include "CenterPoint.h"

/**
 * @brief   Hàm tổ chức danh sách thông tin user thành định dạng cho phép tìm kiếm 
 *          thông tin của bất kỳ user nào 
 * @retval  E_OK    Hàm xử lý thành công
 * @retval  NOT_OK  Hàm xử lý thất bại
 */
Status Build_SearchList();

/**
 * @brief   Hàm tìm user và in ra thông tin cụ thể 
 * @param[in] search  chuỗi ký tự tên hoặc sdt do người dùng nhập và được hàm 
 *                    tự động kiểm tra để xác định định dạng tìm kiếm
 * @retval  E_OK    Hàm xử lý thành công
 * @retval  NOT_OK  Hàm xử lý thất bại
 */
Status Search_User(const char *search);

/**
 * @brief   Hàm xóa bỏ cấu trúc lưu trữ cây nhị phân tìm kiếm user
 * @retval  E_OK    Hàm xử lý thành công
 * @retval  NOT_OK  Hàm xử lý thất bại
 */
Status Remove_SearchList();

#endif


