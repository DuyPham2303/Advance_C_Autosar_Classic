#include <stdio.h>
#include <stdint.h>

// basic features
#define COLOR_RED 0
#define COLOR_BLUE 1
#define COLOR_BLACK 2
#define COLOR_WHITE 3

#define POWER_100HP 0
#define POWER_150HP 1
#define POWER_200HP 2

#define ENGINE_1_5L 0
#define ENGINE_2_0L 1

typedef uint8_t CarColor;
typedef uint8_t CarPower;
typedef uint8_t CarEngine;

// additional features
#define SUNROOF_MASK 1 << 0        
#define PREMIUM_AUDIO_MASK 1 << 1  
#define SPORTS_PACKAGE_MASK 1 << 2

// Thêm các bit masks khác tùy thuộc vào tùy chọn
typedef struct
{
/*
Struct bit field
What ?
    - áp dụng struct only
    - chỉ định số bit cụ thể sử dụng
    - mục tiêu tiết kiệm bộ nhớ
How ?
    - Compiler quy hoạch vùng nhớ -> chỉ rõ bit nào thuộc về field nào
    - Bộ nhớ vẫn cấp phát theo minium unit -> CPU có thể access
    - Compiler đặt các field nào vị trí bit cụ thể
    - các bit thừa chỉ bị bỏ trống (ko bị xóa)
    - CPU khi đọc/ghi
        -> thao tác nguyên byte/word
        -> áp dụng mask (AD/OR/SHIFT) lấy đúng phần bit field tương ứng
    - Ví dụ
        struct {
            a : 1;
            b : 2;
            c : 1;
        }field;
    Bit index   7       6       5   4   3   2   1   0
    nội dung  padding padding   c   b   b   a   a   a
*/
    uint8_t additionalOptions : 3; // 3 bits cho các tùy chọn bổ sung
    CarColor color : 2; 
    CarPower power : 2;
    CarEngine engine : 1;
} CarOptions;

void configureCar(CarOptions *car, CarColor color, CarPower power, CarEngine engine, uint8_t options)
{
    car->color = color;
    car->power = power;
    car->engine = engine;
    car->additionalOptions = options;
}

void setOption(CarOptions *car, uint8_t optionMask)
{
    car->additionalOptions |= optionMask;
}

void unsetOption(CarOptions *car, uint8_t optionMask)
{
    car->additionalOptions &= ~optionMask;
}

void displayCarOptions(const CarOptions car)
{
    const char *colors[] = {"Red", "Blue", "Black", "White"};
    const char *powers[] = {"100HP", "150HP", "200HP"};
    const char *engines[] = {"1.5L", "2.0L"};

    printf("Car Configuration: \n");
    printf("Color: %s\n", colors[car.color]);
    printf("Power: %s\n", powers[car.power]);
    printf("Engine: %s\n", engines[car.engine]);
    printf("Sunroof: %s\n", (car.additionalOptions & SUNROOF_MASK) ? "Yes" : "No");
    printf("Premium Audio: %s\n", (car.additionalOptions & PREMIUM_AUDIO_MASK) ? "Yes" : "No");
    printf("Sports Package: %s\n", (car.additionalOptions & SPORTS_PACKAGE_MASK) ? "Yes" : "No");
}

int main()
{
    // CarOptions myCar = {.color = COLOR_BLACK, .power =  POWER_150HP, .engine = ENGINE_2_0L};
    CarOptions myCar;
    configureCar(&myCar, COLOR_RED, POWER_200HP, ENGINE_2_0L, PREMIUM_AUDIO_MASK);
    setOption(&myCar, SUNROOF_MASK | PREMIUM_AUDIO_MASK | SPORTS_PACKAGE_MASK);

    displayCarOptions(myCar);

    unsetOption(&myCar, PREMIUM_AUDIO_MASK);
    displayCarOptions(myCar);

    // printf("size of my car: %d\n", sizeof(myCar.color));

    return 0;
}
