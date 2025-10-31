#include "parseJson.h"

const char* json_obj = "{      "
                        "\"1001\":{"
                          "\"SoPhong\":3,"
                          "\"NguoiThue\":{"
                            "\"Ten\":\"Nguyen Van A\","
                            "\"CCCD\":\"1920517781\","
                            "\"Tuoi\":26,"
                            "\"isStudent\": true,"
                            "\"ThuongTru\":{"
                              "\"Duong\":\"73 Ba Huyen Thanh Quan\","
                              "\"Phuong_Xa\":\"Phuong 6\","
                              "\"Tinh_TP\":\"Ho Chi Minh\""
                            "}"
                          " },"
                          "\"SoNguoiO\":{"
                            "\"1\":\"Nguyen Van A\","
                            "\"2\":\"Nguyen Van B\","
                            "\"3\":\"Nguyen Van C\""
                          "},"
                          "\"TienDien\": [24, 56, 98],"
                          "\"TienNuoc\":30.000"
                        "},"
                        "\"1002\":{"
                          "\"SoPhong\":5,"
                          "\"NguoiThue\":{"
                            "\"Ten\":\"Phan Hoang Trung\","
                            "\"CCCD\":\"012345678912\","
                            "\"Tuoi\":24,"
                            "\"isStudent\": false,"
                            "\"ThuongTru\":{"
                              "\"Duong\":\"53 Le Dai Hanh\","
                              "\"Phuong_Xa\":\"Phuong 11\","
                              "\"Tinh_TP\":\"Ho Chi Minh\""
                            "}"
                          "},"
                          "\"SoNguoiO\":{"
                            "\"1\":\"Phan Van Nhat\","
                            "\"2\":\"Phan Van Nhi\","
                            "\"2\":\"Phan Van Tam\","
                            "\"3\":\"Phan Van Tu\""
                          "},"
                          "\"TienDien\":23.000,"
                          "\"TienNuoc\":40.000"
                        "}"
                      "}";
const char* json_array = "[null,true,10e4,\"hello\"]";

#define FILE_DIR ".vscode/tasks.json"


char* read_json_file(const char* filename);
int main()
{
    //trả về vùng nhớ lưu trữ dữ liệu json đã xử lý
    const char* json_str = read_json_file(FILE_DIR);

    printf("Nội dung JSON:\n%s\n", json_str);
    
    JsonValue* json_value = parse_json(&json_str); //0xab

    //in ra dữ liệu json 
    printf("NỘI DUNG SAU KHI PHÂN TÁCH\n");
    test(json_value);

    //giải phóng vùng nhớ lưu trữ json
    free_json_value(json_value);
    free((char*)json_str); // giải phóng bộ nhớ khi dùng xong
    json_str = NULL;
    json_value = NULL;
    return 0;
}