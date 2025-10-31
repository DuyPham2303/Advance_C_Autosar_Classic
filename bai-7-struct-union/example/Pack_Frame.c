#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* 
    - kết hợp struct + union để thiết kế cấu trúc dữ liệu truyền đi
    => Union : chứa 2 thành viên chia sẻ chung 1 vùng nhớ
        + struct : gói các thông tin theo từng loại (id,real data,checksum)
        + mảng : chứa dữ liệu thực sự truyền đi (các thông tin từ struct)
         
*/
typedef union
{
    struct
    {
        uint8_t id[2];          //uint16_t id
        uint8_t data[4];        //uint32_t data
        uint8_t check_sum[2];   //uint8_t check_sum
    } data;
    uint8_t frame[8];
} Data_Frame;

// Hàm tính checksum đơn giản: cộng toàn bộ byte (trừ checksum) rồi lấy modulo 256
uint8_t calculate_checksum(uint8_t *payload, size_t len)
{
    uint16_t sum = 0;
    for (size_t i = 0; i < len; ++i)
    {
        sum += payload[i];
    }
    return (uint8_t)(sum % 256);
}

int main(void)
{
    Data_Frame transmit_data = {0};
    
    // Gán dữ liệu (memcpy đảm bảo không vượt kích thước mảng)
    uint8_t id_val[2]   = {0x31, 0x30};      // ASCII '1', '0'
    uint8_t data_val[4] = {0x31, 0x32, 0x33, 0x34}; // ASCII '1' '2' '3' '4'

    memcpy(transmit_data.data.id, id_val, sizeof(id_val));
    memcpy(transmit_data.data.data, data_val, sizeof(data_val));

  /*   // Tính checksum cho 6 byte đầu (id + data)
    uint8_t checksum = calculate_checksum(transmit_data.frame, 6);
    transmit_data.data.check_sum[0] = checksum;
    transmit_data.data.check_sum[1] = 0; // nếu không dùng byte thứ 2 */

    // === Truyền dữ liệu sang thiết bị nhận ===
    Data_Frame receive_data = {0};
    memcpy(receive_data.frame, transmit_data.frame, sizeof(transmit_data.frame));
/* 
    // Kiểm tra checksum tại bên nhận
    uint8_t received_checksum = receive_data.data.check_sum[0];
    uint8_t recomputed_checksum = calculate_checksum(receive_data.frame, 6); */

    printf("== RECEIVED FRAME ==\n");
    printf("ID    : %c%c\n", receive_data.data.id[0], receive_data.data.id[1]);
    printf("DATA  : %c%c%c%c\n", 
           receive_data.data.data[0],
           receive_data.data.data[1],
           receive_data.data.data[2],
           receive_data.data.data[3]);
   /*  printf("CHECKSUM (recv): %u\n", received_checksum);
    printf("CHECKSUM (calc): %u\n", recomputed_checksum);
    
    if (received_checksum == recomputed_checksum)
        printf("✅ Checksum OK. Data integrity verified.\n");
    else
        printf("❌ Checksum MISMATCH. Data corrupted.\n"); */

    return 0;
}
