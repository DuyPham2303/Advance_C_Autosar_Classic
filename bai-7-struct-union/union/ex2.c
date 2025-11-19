#include <stdint.h>
/* 
 - Truy cập memory với nhiều datatype khác nhau
    + Chuyển đổi linh hoạt datatype
    + Hữu ích áp dụng thao tác với dữ liệu truyền/nhận ở cấp độ bit (packing/unpacking)
    + tránh việc tốn tgian ép kiểu
*/
union U32Bytes {
	uint32_t value;
	uint8_t bytes[4];
};

void send_byte(uint8_t byte) {
    printf("0x%02X ", byte);
}

void send32BitValue(uint32_t data) {
	union U32Bytes packet;
	packet.value = data; // Assign a 32-bit value
	
    printf("Sending 32-bit value: 0x%08X\n", packet.value);
    printf("Bytes sent (LSB → MSB, tùy Endianness hệ thống): ");

	send_byte(packet.bytes[0]);
	send_byte(packet.bytes[1]);
	send_byte(packet.bytes[2]);
	send_byte(packet.bytes[3]);
	// Note: Byte order depends on the Endianness of the system
}

uint32_t receive32BitValue(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    union U32Bytes packet;

    // Gán từng byte vào union
    packet.bytes[0] = b0;
    packet.bytes[1] = b1;
    packet.bytes[2] = b2;
    packet.bytes[3] = b3;

    // Đọc lại toàn bộ giá trị 32-bit
    return packet.value;
}

int main(void)
{
    uint32_t original = 0x1234ABCD; // Dữ liệu 32-bit mẫu
    printf("DEMO: Truy cập vùng nhớ bằng nhiều datatype\n");

    // Gửi giá trị qua các byte
    send32BitValue(original);

    // Giả lập nhận lại dữ liệu (các byte nhận được)
    uint8_t rx_bytes[4] = { 0xCD, 0xAB, 0x34, 0x12 }; 
    uint32_t reconstructed = receive32BitValue(rx_bytes[0], rx_bytes[1], rx_bytes[2], rx_bytes[3]);

    printf("\nReconstructed 32-bit value: 0x%08X\n", reconstructed);

    // Kiểm tra kết quả
    if (reconstructed == original)
        printf("Kết quả khớp với giá trị gốc!\n");
    else
        printf("Kết quả KHÔNG khớp (Endianness khác hoặc sai byte order).\n");

    return 0;
}
