#include <stdbool.h>
#include <stdint.h>

// Danh sách liệt kê các kiểu có thế có trong chuỗi json
typedef enum
{
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

// cấu trúc lưu trữ dữ liệu sau khi phân tích từ json
typedef struct JsonValue
{
    JsonType type;
    // tổ hợp chia sẻ vùng nhớ chung cho các member(biến) lưu trữ Json value (do value chỉ gán cho 1 member)
    union
    {
        bool boolean;
        double number;
        char *string;

        // cấu trúc lưu trữ cho mảng json
        struct
        {
            struct JsonValue *values; // con trỏ đệ quy trở lại JsonValue để xác định kiểu (type) và biến tương ứng để lưu trữ value
            size_t count;             // đếm số lượng phần tử trong mảng
        } array;

        // cấu trúc lưu trữ cho đối tượng json
        struct
        {
            char **keys;              // mảng con trỏ lưu trữ các chuỗi key
            struct JsonValue *values; // con trỏ đệ quy trở lại JsonValue để xác định kiểu (type) và biến tương ứng để lưu trữ value
            size_t count;             // đếm số lượng cặp key-value
        } object;
    } value;
} JsonValue;

// [43.23 , "duy pham" , true , [35 , "tuoi"]]

int main()
{
    // cấp phát memory cho 1 đối tương kiểu json
    JsonValue *info_list = (JsonValue *)malloc(sizeof(JsonValue *));
    info_list->type = JSON_ARRAY;     // xác đinh kiểu json là mảng
    info_list->value.array.count = 4; // số lượng thành phần trong mảng

    // cấp phát memory cho các phần tử trong array
    info_list->value.array.values = (JsonValue *)malloc(info_list->value.array.count * sizeof(JsonValue));

    info_list->value.array.values[0].type = JSON_NUMBER;
    info_list->value.array.values[0].value.number = 45.23;

    info_list->value.array.values[1].type = JSON_STRING;
    info_list->value.array.values[1].value.string = "Duysolo";

    info_list->value.array.values[2].type = JSON_BOOLEAN;
    info_list->value.array.values[2].value.boolean = true;


    info_list->value.array.values[3].type = JSON_ARRAY;
    info_list->value.array.values[3].value.array.count = 2; // số lượng phần tử của 1 phần tử có kiểu array

    info_list->value.array.values[3].value.array.values[0].type = JSON_NUMBER;
    info_list->value.array.values[3].value.array.values[0].value.number = 35;
    info_list->value.array.values[3].value.array.values[1].type = JSON_STRING;
    info_list->value.array.values[3].value.array.values[1].value.string = "Tuoi";
    return 0;
}