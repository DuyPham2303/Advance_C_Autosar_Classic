#ifndef CSV_LIB_H
#define CSV_LIB_H

/**
 * @brief   Đọc chuỗi PDU (ví dụ "0x3F0 8 03 13 BD 00 00 00 00 00")
 *          từ file CSV với key là "can".
 *
 * @param   key   Chuỗi khóa (chỉ quan tâm "can").
 * @return  Nếu tồn tại dòng bắt đầu bằng "can," thì trả phần sau dấu ',' (malloc’d).
 *          Caller phải free() sau khi dùng. Nếu không tìm thấy hoặc lỗi, trả NULL.
 */
char *getDataPdu(const char *key);

#endif /* CSV_LIB_H */
