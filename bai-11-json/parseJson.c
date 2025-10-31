#include "parseJson.h"
/// @brief      : hàm bỏ qua khoảng tráng
/// @param json : con trỏ đến con trỏ lưu trữ chuỗi json
/// @return     : void

static void skip_whitespace(const char **json) {
    while (isspace(**json)) {
        (*json)++;
    }
}
/// @brief      : hàm phân tích ký tự null
/// @param json : con trỏ đến con trỏ lưu trữ chuỗi json
/// @return     : con trỏ đến dữ liệu json chứa ký tự null đã xử lý có kiểu JsonValue
static JsonValue *parse_null(const char **json) {
    //strcmp(str1,str2)
    if (strncmp(*json, "null", 4) == 0) {
        JsonValue *value = (JsonValue *) malloc(sizeof(JsonValue));
        value->type = JSON_NULL;
        *json += 4;
        return value;
    }
    return NULL;
}

/// @brief      : hàm phân tích giá trị boolean
/// @param json : con trỏ đến con trỏ lưu trữ chuỗi json
/// @return     : con trỏ đến dữ liệu json chứa giá trị boolean đã xử lý có kiểu NULL
static JsonValue *parse_boolean(const char **json) {
    JsonValue *value = (JsonValue *) malloc(sizeof(JsonValue));
    if (strncmp(*json, "true", 4) == 0) {
        value->type = JSON_BOOLEAN;
        value->value.boolean = true;
        *json += 4;
    } else if (strncmp(*json, "false", 5) == 0) {
        value->type = JSON_BOOLEAN;
        value->value.boolean = false;
        *json += 5;
    } else {
        free(value);
        return NULL;
    }
    return value;
}

/// @brief      : hàm phân tích 1 giá trị số 
/// @param json : con trỏ đến con trỏ lưu trữ chuỗi json
/// @return     : con trỏ đến dữ liệu json chứa dãy số đã xử lý có kiểu double
static JsonValue *parse_number(const char **json) {
   //"123abc"
   /* 
        'a'     '2'     '3'     'a'     
        0xa1    0xa2    0xa3    0xa4
        *json                   &end
        
   */
   char *end;                        //con trỏ tới cuối chuỗi ký 
   double num = strtod(*json, &end); //hàm để chuyển đổi chuỗi sang kiểu số double 
   //nếu địa chỉ lưu trong con trỏ end khác *json thì tiến hành cấp phát vùng nhớ và gán giá trị trả về
   if (end != *json) {
       JsonValue *value = (JsonValue *) malloc(sizeof(JsonValue));
       value->type = JSON_NUMBER;
       value->value.number = num;
       *json = end; //cập nhật địa chỉ của chuỗi json 
       return value;
   }
   return NULL;
}

/// @brief      : hàm phân tích 1 chuỗi ký tự
/// @param json : con trỏ đến con trỏ lưu trữ chuỗi json
/// @return     : con trỏ đến dữ liệu json chứa chuỗi ký tự đã xử lý có kiểu char*
static JsonValue *parse_string(const char **json) {
	(*json)++;	
    skip_whitespace(json);								 //dịch tới địa chỉ tiếp theo để bắt đầu xử lý từ ký tự đầu tiên 
	/* 
        h       e       l       l       0       \"
        0x01    0x02    0x03    0x04    0x05
        *json
        start
                                                end
    */
   
    if(**json == '\"'){
        (*json)++;
    }

    const char *start = *json;					 //lưu địa chỉ bắt đầu của chuỗi 
	const char *end = start;                     //biến để kiểm tra độ dài chuỗi
    while(*end!= '\"' && *end!= '\0') {
		end++; 							     //kiểm tra độ dài chuỗi
	}
	if (*end == '\"') {
		size_t length = end - start;           //cập nhật kích thước chuỗi

    //cấp phát vùng nhớ và lưu tạm thời chuỗi vừa tách được 
		char *str = (char *) malloc((length + 1) * sizeof(char)); 
		strncpy(str, start, length);
		str[length] = '\0';

    //cấp phát vùng nhớ để lưu chuỗi trả về thực sự
		JsonValue *value = (JsonValue *) malloc(sizeof(JsonValue));
		value->type = JSON_STRING; //xác định kiểu dữ liệu 
		value->value.string = str; //gán vào biến tương ứng
		*json = ++end;             //cập nhật lại địa chỉ chuỗi json để xử lý chuỗi kế tiếp
        //free(str);
		return value;
	}
    return NULL;
}

/// @brief      : hàm phân tích 1 mảng json
/// @param json : con trỏ đến con trỏ lưu trữ chui json
/// @return     : con trỏ đến dữ liệu json chứa mảng json đã xử lý có kiểu struct array
static JsonValue *parse_array(const char **json) {
   (*json)++;                       //dịch sang địa chỉ tiếp theo
   skip_whitespace(json);           //bỏ qua khoảng trắng

    //cấp phát vùng nhớ ban đầu cho mảng và khởi tạo các giá trị ban đầu
   JsonValue *array_value = (JsonValue *)malloc(sizeof(JsonValue));
   array_value->type = JSON_ARRAY;
   array_value->value.array.count = 0;
   array_value->value.array.values = NULL;


   while (**json != ']' && **json != '\0') {

       JsonValue *element = parse_json(json);  

       if (element) {
           array_value->value.array.count++;
         
           array_value->value.array.values = 
           (JsonValue *)realloc(array_value->value.array.values, array_value->value.array.count * sizeof(JsonValue));
      
           array_value->value.array.values[array_value->value.array.count - 1] = *element;
           
           free(element); 

       }
       else { 
           break; 
       }
       skip_whitespace(json); //nếu có khoảng tráng sau mỗi phần tử trong chuỗi thì bỏ qua

       
       if (**json == ',') {
           (*json)++;   //dịch địa chỉ sang phần tử kế tiếp
       }
   }

   if (**json == ']') {
       (*json)++;           //dịch địa chỉ kế tiếp sau khi kết thúc phân tích mảng
       return array_value;
   } 
   //nếu ký tự kết thúc mảng không hợp lệ thì sẽ giải phóng vùng nhớ và trả về NULL
   else {
       free_json_value(array_value);
       return NULL;
   }
   return NULL;
}

/// @brief      : hàm phân tích 1 đối tượng json
/// @param json : con trỏ đến con trỏ lưu trữ chuỗi json
/// @return     : con trỏ đến dữ liệu json chứa đối tượng json đã xử lý có kiểu struct Object
static JsonValue *parse_object(const char **json) {
    (*json)++;
    skip_whitespace(json);

    JsonValue *object_value = (JsonValue *)malloc(sizeof(JsonValue));
    object_value->type = JSON_OBJECT;
    object_value->value.object.count = 0;
    object_value->value.object.keys = NULL;
    object_value->value.object.values = NULL;

    while (**json != '}' && **json != '\0') {
        JsonValue *key = parse_string(json); //temp var -> will be free after assign to final result
        if (key) {
            skip_whitespace(json);
            if (**json == ':') {
                (*json)++;
                JsonValue *value = parse_json(json); //temp var -> will be free after assign to final result
                if (value) {
                    object_value->value.object.count++;
                    object_value->value.object.keys = (char **)realloc(object_value->value.object.keys, object_value->value.object.count * sizeof(char *));
                    object_value->value.object.keys[object_value->value.object.count - 1] = key->value.string;

                    object_value->value.object.values = (JsonValue *)realloc(object_value->value.object.values, object_value->value.object.count * sizeof(JsonValue));
                    object_value->value.object.values[object_value->value.object.count - 1] = *value;
                    free(value);
                } else {
                    free_json_value(key);
                    break;
                }
            } else {
                free_json_value(key);
                break;
            }
        } else {
            break;
        }
        skip_whitespace(json);
        if (**json == ',') {
            (*json)++;
        }
    }
    if (**json == '}') {
        (*json)++;
        return object_value;
    } else {
        free_json_value(object_value);
        return NULL;
    }
    return NULL;
}

JsonValue *parse_json(const char **json) { 
    //json : 0xb3 = 0xab (&son_obj)
    //"abcef"
    //'a'   'b'     'c'     'd'
    //0xa1  0xa2    0xa3    0xa4
    //json -> 0xab
    //(*json)++ -> (0xa2)
    //**json -> 'c'
    //json_obj -> (0xa2)
    skip_whitespace(json);
    switch (**json) {
        case 'n':
            return parse_null(json); //return nullif there is no json string is existed
        case 't':
        case 'f':
            return parse_boolean(json);
        case '\"':
            return parse_string(json);
        case '[':
            return parse_array(json);
        case '{':
            return parse_object(json);
        default:
            if (isdigit(**json) || **json == '-') { 

                return parse_number(json);
            } else {
                // Lỗi phân tích cú pháp
                return NULL;
            }
    }
}

void free_json_value(JsonValue *json_value) {
    if (json_value == NULL) {
        return;
    }
    switch (json_value->type) {
        case JSON_STRING:
            free(json_value->value.string);
            break;
        case JSON_ARRAY:
            for (size_t i = 0; i < json_value->value.array.count; i++) {
                free_json_value(&json_value->value.array.values[i]);
            }
            free(json_value->value.array.values);
            break;
        case JSON_OBJECT:
            for (size_t i = 0; i < json_value->value.object.count; i++) {
                free(json_value->value.object.keys[i]);
                free_json_value(&json_value->value.object.values[i]);
            }
            free(json_value->value.object.keys);
            free(json_value->value.object.values);
            break;
        default:
            break;
    }
}

void test(JsonValue* json_value){
    if (json_value != NULL && json_value->type == JSON_OBJECT) {
        // Truy cập giá trị của các trường trong đối tượng JSON
        size_t num_fields = json_value->value.object.count;

        for (size_t i = 0; i < num_fields; ++i) {

            char* key = json_value->value.object.keys[i];
            JsonValue* value = &json_value->value.object.values[i];

            JsonType type = (int)(json_value->value.object.values[i].type);


            if(type == JSON_STRING){
                printf("%s: %s\n", key, value->value.string);
            }

            else if(type == JSON_NUMBER){
                printf("%s: %.1f\n", key, value->value.number);
            }

            else if(type == JSON_BOOLEAN){
                printf("%s: %s\n", key, value->value.boolean ? "True":"False");
            }

            else if(type == JSON_OBJECT){
                printf("%s: \n", key);
                test(value);
            }

            else if(type == JSON_ARRAY){
                printf("%s: ", key);
                for (size_t i = 0; i < value->value.array.count; i++)
                {
                   test(value->value.array.values + i);
                } 
                printf("\n");
            }
        }
     
    }
    else 
    {
            if(json_value->type == JSON_STRING){
                printf("%s ", json_value->value.string);
            }

            else if(json_value->type == JSON_NUMBER){
                printf("%.1f ", json_value->value.number);
            }

            else if(json_value->type == JSON_BOOLEAN){
                printf("%s ", json_value->value.boolean ? "True":"False");
            }

            else if(json_value->type == JSON_OBJECT){
                printf("%s: \n", *(json_value->value.object.keys));
                test(json_value->value.object.values);
                           
            }
            
            else if(json_value->type == JSON_ARRAY){
                for (size_t i = 0; i < json_value->value.array.count; i++)
                {
                   test(json_value->value.array.values + i);
                } 
                printf("\n");
            }
            else if (json_value->type ==JSON_NULL){
                printf("null ");
            }
    }

}


