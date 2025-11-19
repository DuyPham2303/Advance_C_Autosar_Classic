#include <stdint.h>

/*
Tiết kiệm Bộ nhớ với Union
    + Lưu nhiều loại datatype khác nhau
    + Tại 1 thời điểm chỉ cần lưu 1 trong số chúng
*/
enum MessageType
{
    SENSOR_DATA_MSG,
    CONTROL_CMD_MSG
};

typedef struct Message
{
    enum MessageType type;
    union
    {
        struct
        {
            float temperature;
            float humidity;
        } sensorData;
        struct
        {
            uint8_t commandId;
            uint16_t value;
        } controlCmd;
    } payload; // payload
} Message;

int main()
{

    Message msg;
    // Trường hợp 1: Gửi dữ liệu cảm biến (SENSOR_DATA_MSG)
    msg.type = SENSOR_DATA_MSG;
    msg.payload.sensorData.temperature = 25.5f;
    msg.payload.sensorData.humidity = 60.2f;

    printf("SENSOR DATA MESSAGE\n");
    printf("Type: SENSOR_DATA_MSG\n");
    printf("Temperature: %.2f °C\n", msg.payload.sensorData.temperature);
    printf("Humidity   : %.2f %%\n", msg.payload.sensorData.humidity);

    // Trường hợp 2: Gửi lệnh điều khiển (CONTROL_CMD_MSG)
    msg.type = CONTROL_CMD_MSG;
    msg.payload.controlCmd.commandId = 0xA1;
    msg.payload.controlCmd.value = 1234;

    printf("\nCONTROL COMMAND MESSAGE\n");
    printf("Type: CONTROL_CMD_MSG\n");
    printf("Command ID: 0x%X\n", msg.payload.controlCmd.commandId);
    printf("Value     : %u\n", msg.payload.controlCmd.value);
}