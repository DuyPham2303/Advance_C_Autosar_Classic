#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define GENDER 1 << 0 // Bit 0: Giới tính (0 = Nữ, 1 = Nam) // 0b 0000 0001 
#define TSHIRT 1 << 1 // Bit 1: Áo thun (0 = Không, 1 = Có) // 0b 0000 0010
#define HAT    1 << 2    // Bit 2: Nón (0 = Không, 1 = Có)  // 0b 0000 0100
#define SHOES  1 << 3  // Bit 3: Giày (0 = Không, 1 = Có)   // 0b 0000 1000

// Tự thêm 5 tính năng khác
#define FEATURE1 1 << 4 // Bit 4: Tính năng 1
#define FEATURE2 1 << 5 // Bit 5: Tính năng 2
#define FEATURE3 1 << 6 // Bit 6: Tính năng 3
#define FEATURE4 1 << 7 // Bit 7: Tính năng 4

void enableFeature(uint8_t *features, uint8_t feature)
{
    *features = *features | feature;
}
void removeFeatures(uint8_t *features, uint8_t unwantedFeatures)
{
    *features = *features &  ~unwantedFeatures;
}
bool isFeatureEnabled(uint8_t features, uint8_t feature) //0xab
{
    return (((features & feature) != 0) ? true : false);
}
void listSelectedFeatures(uint8_t features)
{
    printf("Selected Features:\n");

    if (features & GENDER)
    {
        printf("- Gender\n");
    }
    if (features & TSHIRT)
    {
        printf("- T-Shirt\n");
    }
    if (features & HAT)
    {
        printf("- Hat\n");
    }
    if (features & SHOES)
    {
        printf("- Shoes\n");
    }
}

int main()
{
    uint8_t options = 0; //0x3f
    // Thêm tính năng
    enableFeature(&options, GENDER | TSHIRT | HAT | SHOES); // 1 << 0 | 1 << 1 | 1 << 2 | 1<< 3 => 0b 0000 1111

    listSelectedFeatures(options);

    printf("feature TSHIRT is %s\n", isFeatureEnabled(options, TSHIRT) ? "ENALBE" : "DISABLED");
    removeFeatures(&options, TSHIRT | HAT);
    printf("feature TSHIRT is %s\n", isFeatureEnabled(options, TSHIRT) ? "ENALBE" : "DISABLED");


    return 0;
}
