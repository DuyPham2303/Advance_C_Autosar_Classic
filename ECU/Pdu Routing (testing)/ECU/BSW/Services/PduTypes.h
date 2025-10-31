#ifndef PDUTYPES_H
#define PDUTYPES_H

#include <stdint.h>

#define PDU_MAX_DATA_LEN 8U

typedef uint32_t    Can_IdType;
typedef uint8_t     PduLengthType;

/**
 * @brief   Một PDU chung, chứa CAN ID, độ dài và dữ liệu SDU tối đa 8 byte.
 */
typedef struct {
    Can_IdType      id;         /* Identifier (ví dụ 0x3F0) */
    PduLengthType   length;     /* Số byte dữ liệu thực (0..8) */
    uint8_t         sdu[PDU_MAX_DATA_LEN];  /* Mảng dữ liệu */
    uint32_t        swPduHandle;            /* Không dùng trong ví dụ */
} PduInfoType;

#endif /* PDUTYPES_H */
