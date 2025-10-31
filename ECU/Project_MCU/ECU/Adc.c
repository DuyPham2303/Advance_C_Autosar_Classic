#include "Adc.h"

// Giả lập ADC hardware với các giá trị cấu hình
static Adc_ConfigType Adc_CurrentConfig;  // Lưu trữ cấu hình hiện tại của ADC

// Hàm khởi tạo ADC với cấu trúc cấu hình
void Adc_Init(const Adc_ConfigType* ConfigPtr) {
    if (ConfigPtr == NULL) {
        printf("Error: Null configuration pointer passed to Adc_Init.\n");
        return;
    }

    // Lưu cấu hình ADC từ ConfigPtr vào biến toàn cục
    Adc_CurrentConfig.Adc_Channel = ConfigPtr->Adc_Channel;
    Adc_CurrentConfig.Adc_SamplingRate = ConfigPtr->Adc_SamplingRate;
    Adc_CurrentConfig.Adc_Resolution = ConfigPtr->Adc_Resolution;

    // Khởi tạo seed cho việc sinh số ngẫu nhiên để mô phỏng ADC
    srand(time(0));

    // In ra thông tin cấu hình ADC
    printf("ADC Initialized with Configuration:\n");
    printf(" - Channel: %d\n", Adc_CurrentConfig.Adc_Channel);
    printf(" - Sampling Rate: %d Hz\n", Adc_CurrentConfig.Adc_SamplingRate);
    printf(" - Resolution: %d-bit\n", Adc_CurrentConfig.Adc_Resolution);
}


uint16_t adcValuetest;

Adc_ReadChannel(ADC_CHANNEL_0, &adcValuetest);


// Đọc giá trị từ một kênh ADC cụ thể (với giá trị ngẫu nhiên)
int Adc_ReadChannel(int channel, uint16_t* adcValue) {


    // Trả giá trị đọc được thông qua tham chiếu
    *adcValue = getData("temp");

    return E_OK;  // Trả về E_OK nếu đọc thành công
}

