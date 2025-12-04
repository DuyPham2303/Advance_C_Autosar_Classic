#ifndef OS_TYPES_H
#define OS_TYPES_H

#include <stdint.h>   /* uint8_t, uint16_t, uint32_t */

/* =====================================================================================
 * 1) Kiểu cơ bản
 * -------------------------------------------------------------------------------------
 *  boolean     : kiểu TRUE/FALSE theo AUTOSAR (8-bit)
 *  StatusType  : mã trả về của API OS (8-bit)
 *  EventMaskType : mặt nạ Event (16-bit)
 *  TickType    : đơn vị thời gian trong mô phỏng (ms, 32-bit)
 * ===================================================================================== */
typedef uint8_t   boolean;
typedef uint8_t   StatusType;
typedef uint16_t  EventMaskType;
typedef uint32_t  TickType;

/* Giá trị logic chuẩn AUTOSAR cho boolean */
#ifndef TRUE
#  define TRUE   ((boolean)1u)
#  define FALSE  ((boolean)0u)
#endif

/* =====================================================================================
 * 2) Mã lỗi/hoàn thành cơ bản của OS (subset đủ dùng cho mô phỏng)
 * -------------------------------------------------------------------------------------
 *  E_OK       : gọi API thành công
 *  E_OS_LIMIT : vượt giới hạn (ví dụ ActivateTask khi task đang chạy)
 *  E_OS_ID    : tham số ID không hợp lệ (task/alarm/event)
 *  E_OS_STATE : trạng thái hệ không phù hợp hoặc lỗi nền tảng (pthread/create…)
 * ===================================================================================== */
#define E_OK         ((StatusType)0u)
#define E_OS_LIMIT   ((StatusType)1u)
#define E_OS_ID      ((StatusType)2u)
#define E_OS_STATE   ((StatusType)3u)


#endif /* OS_TYPES_H */