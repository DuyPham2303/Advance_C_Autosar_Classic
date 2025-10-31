/********************************************************
 *******************
 * @file SharedData.h
 * @brief Khai báo các hàm chia sẻ dữ liệu an toàn giữa nhiều file trong module Service
 * @details File này cung cấp định nghĩa các hàm Read_UserDB, Read_SizeUserDB, Read_ListUser
 *          và 1 số hàm khác, Header này chỉ nên được include cục bộ ở module hiện tại 
 * @version 1.0
 * @date 2025-16-8
 * @author Pham Cao Duy
 *********************************************************
 ******************/
#ifndef SHARED_DATA_H
#define SHARED_DATA_H
#include "UserData.h"
#include "UserNode.h"

/**
 * @brief   hàm đọc giá trị số lượng User  
 * @retval  giá trị số nguyên cho biết số lượng UserData
 */
int Read_SizeUserDB();  //định nghĩa ở UserData.c
/**
 * @brief   hàm đọc dữ liệu của các user đã lưu trữ có kiểu UserData
 * @retval  NULL    Nếu con trỏ đến UserData* không hợp lệ  
 */
UserData* Read_UserDB(); //định nghĩa ở UserData.c
/**
 * @brief       Hàm đọc dữ liệu của các user lưu ở dạng linked list có kiểu UserNode
 * @param[in]   head    biến enum xác định loại linked list sẽ trả về dựa trên tên/sdt
 * @retval  NULL    Nếu con trỏ đến UserNode* không hợp lệ  
 */
UserNode* Read_ListUser(SortType type); //định nghĩa ở UserNode.c
#endif