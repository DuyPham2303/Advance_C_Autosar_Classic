#include <stdio.h>
#include <stdint.h>

// Macro LOG với __VA_ARGS__ => in log, thêm tên file, dòng, và module.
#define LOG(fmt, ...)  \
    printf("[LOG][%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// Macro nối chuỗi ## => Tự động sinh tên biến thanh ghi / sensor
#define SENSOR_REG(name)   SENSOR_##name##_REG
#define DEFINE_SENSOR_REG(name, addr)  \
    uint32_t SENSOR_##name##_REG = addr

// Macro chuẩn hóa chuỗi # => Chuyển tên biến thành chuỗi để in ra
#define SHOW_VAR(x)  printf(#x " = %d\n", x)

// =======================
// Ứng dụng thực tế: Mô phỏng hệ thống đọc cảm biến
// =======================

// Tạo địa chỉ thanh ghi cho từng cảm biến
DEFINE_SENSOR_REG(TEMP, 0x4000);
DEFINE_SENSOR_REG(PRESSURE, 0x4004);
DEFINE_SENSOR_REG(HUMID, 0x4008);

// Hàm giả lập đọc cảm biến
uint16_t read_sensor(uint32_t addr) {
    // Giả sử phần cứng trả về giá trị ngẫu nhiên
    return (addr % 100) + 20;
}

void app_sensor_read() {
    uint16_t temp = read_sensor(SENSOR_REG(TEMP));
    uint16_t pressure = read_sensor(SENSOR_REG(PRESSURE));
    uint16_t humid = read_sensor(SENSOR_REG(HUMID));

    LOG("Reading sensors...");
    SHOW_VAR(temp);
    SHOW_VAR(pressure);
    SHOW_VAR(humid);
}

int main() {
    app_sensor_read();
    return 0;
}
