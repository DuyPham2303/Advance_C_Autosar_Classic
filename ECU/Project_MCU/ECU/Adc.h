#ifndef ADC_H
#define ADC_H

#include <stdio.h>
#include <stdlib.h>  // Thư viện hỗ trợ tạo giá trị ngẫu nhiên
#include <time.h>    // Thư viện hỗ trợ thời gian (sử dụng cho random)
#include <unistd.h>  // Thư viện hỗ trợ hàm sleep (sử dụng cho delay)
#include "Std_Types.h"

// Cấu trúc chứa thông tin cấu hình của ADC
typedef struct {
    uint8_t Adc_Channel;       // Kênh ADC cần khởi tạo
    uint32_t Adc_SamplingRate; // Tần số lấy mẫu
    uint8_t Adc_Resolution;    // Độ phân giải ADC (ví dụ: 8, 10, 12 bit)
} Adc_ConfigType;



#define ADC_CHANNEL_0 = 0,     /**< Kênh ADC 0, ví dụ PA0 */
#define   ADC_CHANNEL_1 = 1,     /**< Kênh ADC 1, ví dụ PA1 */
    ADC_CHANNEL_2 = 2,     /**< Kênh ADC 2, ví dụ PA2 */
    ADC_CHANNEL_3 = 3,     /**< Kênh ADC 3, ví dụ PA3 */
    ADC_CHANNEL_4 = 4,     /**< Kênh ADC 4, ví dụ PA4 */


    Adc_ConfigType test = {ADC_CHANNEL_0, 50, 10};


    Adc_Init(&test);


void Adc_Init(const Adc_ConfigType* ConfigPtr);

// Đọc giá trị từ ADC kênh cụ thể
int Adc_ReadChannel(int channel, uint16_t* adcValue);


#endif // ADC_H
