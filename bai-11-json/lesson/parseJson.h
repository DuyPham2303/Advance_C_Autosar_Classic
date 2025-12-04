#ifndef PARSEJSON_H
#define PARSEJSON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

//Danh sách liệt kê các kiểu có thế có trong chuỗi json
typedef enum {
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

//cấu trúc lưu trữ dữ liệu sau khi phân tích từ json
typedef struct JsonValue {
    JsonType type;

    //tổ hợp chia sẻ vùng nhớ chung cho các member(biến) lưu trữ Json value (do value chỉ gán cho 1 member)
    union { 
        bool boolean;                   
        double number;
        char *string; 

        //cấu trúc lưu trữ cho mảng json
        struct {
            struct JsonValue *values; //con trỏ đệ quy trở lại JsonValue để xác định kiểu (type) và biến tương ứng để lưu trữ value 
            size_t count;             //đếm số lượng phần tử trong mảng
        } array;

        //cấu trúc lưu trữ cho đối tượng json
        struct {
            char **keys;              //mảng con trỏ lưu trữ các chuỗi key
            struct JsonValue *values; //con trỏ đệ quy trở lại JsonValue để xác định kiểu (type) và biến tương ứng để lưu trữ value 
            size_t count;             //đếm số lượng cặp key-value
        } object;
    } value;
}JsonValue;


/// @brief      : hàm phân tích kiểu json tương ứng để xử lý
/// @param json : con trỏ đến con trỏ lưu trữ chuỗi json
/// @return     : con trỏ đến dữ liệu json đã phân tích có kiểu JsonValue
JsonValue *parse_json(const char **json);

/// @brief      : hàm in ra dữ liệu json sau khi đã phân tích
/// @param json : con trỏ đến dữ liệu json đã lưu trữ
/// @return     : void
void test(JsonValue* json_value);

/// @brief      : hàm giải phóng vùng nhớ lưu trữ dữ liệu json
/// @param json : con trỏ đến dữ liệu json đã lưu trữ
/// @return     : void
void free_json_value(JsonValue *json_value);


#endif