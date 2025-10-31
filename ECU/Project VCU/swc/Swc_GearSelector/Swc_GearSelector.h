#ifndef SWC_GEARSELECTOR_H
#define SWC_GEARSELECTOR_H

/*
 * SWC: GearSelector (DRNP)
 * Nhiệm vụ:
 *   - Đọc tối đa 4 kênh số của cần số: P, R, N, D (qua RTE → IoHwAb Digital).
 *   - Debounce phần mềm từng kênh.
 *   - Ánh xạ mức điện áp theo cực active_high → trạng thái “đang chọn”.
 *   - Giải quyết xung đột bằng bảng ưu tiên, rồi ghi Gear_e ra RTE.
 *
 * Giao tiếp:
 *   - Client/Server: Rte_Call_DigIf_ReadChannel(uint8 ch, boolean* level)
 *   - Sender/Receiver (Provide): Rte_Write_GearOut_Gear(Gear_e gear)
 */

#include "Std_Types.h"
#include "Rte_Type.h"
#include "Rte_Swc_GearSelector.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Giá trị kênh đặc biệt: 255 = không dùng (unconnected) */
static const uint8 SWC_GEARSEL_CHANNEL_UNUSED = 255u;

/* Cấu hình runtime cho SWC GearSelector */
typedef struct
{
    /* ID kênh đọc từ IoHwAb_Digital; đặt = SWC_GEARSEL_CHANNEL_UNUSED nếu không dùng */
    uint8 ch_P;
    uint8 ch_R;
    uint8 ch_N;
    uint8 ch_D;

    /* Cực mức “đang chọn” (TRUE: mức 1 nghĩa là P/R/N/D đang được chọn) */
    boolean active_high;

    /* Số mẫu liên tiếp (chu kỳ 10ms) để xác nhận đổi trạng thái mỗi kênh */
    uint8 debounce_ticks;

    /* Số mặc định khi tín hiệu không hợp lệ (ví dụ khởi tạo) */
    Gear_e default_gear;

    /* Bảng ưu tiên khi nhiều kênh cùng được chọn (4 phần tử, thứ tự ưu tiên từ cao đến thấp) */
    Gear_e resolve_priority[4];
} Swc_GearSelector_ConfigType;

/* Khởi tạo SWC với cấu hình runtime */
void Swc_GearSelector_Init(const Swc_GearSelector_ConfigType* cfg);

/* Runnable 10ms (được OS/RTE gọi theo lịch) */
void Swc_GearSelector_Run10ms(void);

#ifdef __cplusplus
}
#endif

#endif /* SWC_GEARSELECTOR_H */
