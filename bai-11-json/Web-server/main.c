#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_REQUEST 4096

//Hàm đọc và trả về buffer chứa nội dung file 
char* read_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    // Tìm kích thước file
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f); // Quay lại đầu file

    // Cấp phát bộ nhớ
    char *content = malloc(size + 1);
    if (!content) {
        fclose(f);
        return NULL;
    }

    fread(content, 1, size, f);
    content[size] = '\0';  // Kết thúc chuỗi

    fclose(f);
    return content;
}

//hàm gửi phản hồi bởi server về client
void send_response(SOCKET client, const char *status, const char *type, const char *body)
{
    char response[MAX_REQUEST];
    sprintf(response,
            "HTTP/1.1 %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %d\r\n"
            "\r\n"
            "%s",
            status, type, (int)strlen(body), body);
    send(client, response, strlen(response), 0);
}

//hàm do server xử lý để lấy kích thước của message request bởi client
int get_content_length(const char *request)
{
    const char *cl = strstr(request, "Content-Length:");
    if (!cl)
        return 0;
    int len = 0;
    sscanf(cl, "Content-Length: %d", &len);
    return len;
}

//hàm để lấy nội dung cụ thể của message request 
char *get_body(const char *request)
{
    const char *body = strstr(request, "\r\n\r\n");
    return body ? (char *)(body + 4) : NULL;
}

//hàm để lưu dữ liệu vào file 
void save_data_to_file(const char *data)
{
    FILE *f = fopen("saved_data.json", "a");
    if (f)
    {
        fprintf(f, "%s\n", data);
        fclose(f);
    }
}

//hàm để mã hóa dữ liệu thành bố cục json
void save_form_encoded_as_json(const char *form_data) {
    char temp[32], voltage[32];
    sscanf(form_data, "temp=%[^&]&voltage=%s", temp, voltage);

    char json_line[128];
    sprintf(json_line, "{\"temp\":%s, \"voltage\":%s}", temp, voltage);
    printf("temp = %s\tvolatage : %s\n",temp,voltage);
    FILE *f = fopen("saved_data.json", "a");
    if (f) {
        fprintf(f, "%s\n", json_line);
        fclose(f);
    }
}

//hàm xử lý client request 
void *handle_client(void *arg)
{
    SOCKET client = *(SOCKET *)arg;
    free(arg); // Giải phóng vùng nhớ chứa socket

    //buffer chứa message từ client request 
    char buffer[MAX_REQUEST];
    int len = recv(client, buffer, sizeof(buffer) - 1, 0);
    if (len <= 0)
    {
        closesocket(client);
        return NULL;
    }
    buffer[len] = '\0';

    char method[8], path[256];
    sscanf(buffer, "%s %s", method, path);

    //hiển thị giao diện phản hồi sau khi gửi có url : http://localhost:8080/save-data
    if (strcmp(method, "POST") == 0 && strcmp(path, "/save-data") == 0)
    {
        //tiến hành trích xuất ra nội dung và kích thước của buffer 
        int content_length = get_content_length(buffer);
        char *body = get_body(buffer);

        // Đọc thêm nếu body chưa đủ
        int body_len = strlen(body);
        while (body_len < content_length)
        {
            int r = recv(client, buffer + len, sizeof(buffer) - len - 1, 0);
            if (r <= 0)
                break;
            len += r;
            buffer[len] = '\0';
            body = get_body(buffer);
            body_len = strlen(body);
        }

        if (body && strlen(body) > 0)
        {
            save_form_encoded_as_json(body);
            send_response(client, "200 OK", "text/html; charset=UTF-8","<meta charset='UTF-8'><h2>Gửi thành công!</h2><a href='/'>Quay lại</a>");
        }
        else {
            send_response(client, "400 Bad Request", "text/plain; charset=UTF-8", "Không nhận được dữ liệu");
        }
    }
    
    //hiển thị giao diện tương tác khi truy cập : http://localhost:8080/
    else if (strcmp(method, "GET") == 0 && strcmp(path, "/") == 0) {
        char *html = read_file("data.html");
        if (html) {
            send_response(client, "200 OK", "text/html", html);
            free(html);
        }
        else {
            send_response(client, "404 Not Found", "text/plain; charset=UTF-8", "Không tìm thấy data.html");
        }
    }
    
    else
    {
        send_response(client, "404 Not Found", "text/plain", "Not Found");
    }

    closesocket(client);
    return NULL;
}

int main()
{
    WSADATA wsa;
    SOCKET server, *client_ptr;
    struct sockaddr_in addr;

    //Khởi tạo Winsock - Khởi động TCP connection giữa client và server 
    WSAStartup(MAKEWORD(2, 2), &wsa);

    //Khởi tạo socket - thiết lập kết nối 
    server = socket(AF_INET, SOCK_STREAM, 0);

    //cấu hình địa chỉ server 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    //gán socket vào cấu hình địa chỉ server 
    bind(server, (struct sockaddr *)&addr, sizeof(addr));

    //chờ kết nối
    listen(server, 5);

    printf("🟢 Server đang chạy trên http://localhost:8080/\n");

    //vòng lặp giữ kết nối liên tục
    while (1)
    {
        //server cháp nhận kết nối từ client trước khi giao tiếp
        SOCKET client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET)
            continue;

        client_ptr = malloc(sizeof(SOCKET));
        *client_ptr = client;

        //bắt đầu quá trình giao tiếp TCP giữa client - erver 
        handle_client(client_ptr);
        printf("received from server\n");
    }
    closesocket(server);
    WSACleanup();
    return 0;
}
