#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <pthread.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_REQUEST 4096
pthread_mutex_t mutex;
int count = 0;
//đọc nhiều định dạng file :HTML,CSS,JS,JSON
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

int get_content_length(const char *request)
{
    const char *cl = strstr(request, "Content-Length:");
    if (!cl)
        return 0;
    int len = 0;
    sscanf(cl, "Content-Length: %d", &len);
    return len;
}

char *get_body(const char *request)
{
    const char *body = strstr(request, "\r\n\r\n");
    return body ? (char *)(body + 4) : NULL;
}

void save_data_to_file(const char *data)
{
    FILE *f = fopen("saved_data.json", "a");
    if (f)
    {
        fprintf(f, "%s\n", data);
        fclose(f);
    }
}
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

void *handle_client(void *arg)
{
    SOCKET client = *(SOCKET *)arg;
    free(arg); // Giải phóng vùng nhớ chứa socket

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
            send_response(client, "200 OK", "text/html", "<h2>✅ Gửi thành công!</h2><a href='/'>Quay lại</a>");
        }
        else {
            send_response(client, "400 Bad Request", "text/plain", "Không nhận được dữ liệu");
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
            send_response(client, "404 Not Found", "text/plain", "Không tìm thấy index.html");
        }
    }
    else
    {
        send_response(client, "404 Not Found", "text/plain", "Not Found");
    }

    closesocket(client);
    return NULL;
}
void *print(void *task)
{
    int delay = 0;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        printf("%s is running %d\n", (char*)task, count++);
        if (strcmp(task, "task 1"))
            delay = 500;
        else if (strcmp(task, "task 2"))
            delay = 250;
        else
            delay = 1500;
        pthread_mutex_unlock(&mutex);
        Sleep(delay);
    }
}

int main()
{
    WSADATA wsa;
    SOCKET server, *client_ptr;
    struct sockaddr_in addr;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (struct sockaddr *)&addr, sizeof(addr));
    listen(server, 5);

    printf("🟢 Server đang chạy đa luồng trên http://localhost:8080/\n");
    pthread_t tid1, tid2, tid3;
    const char* task1 = "task 1";
    const char* task2 = "task 2";
    const char* task3 = "task 3";
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&tid1, NULL, print,(void*)task1);
    pthread_create(&tid2, NULL, print,(void*)task2);
    pthread_create(&tid3, NULL, print,(void*)task3);
    while (1)
    {
        SOCKET client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET)
            continue;

        client_ptr = malloc(sizeof(SOCKET));
        *client_ptr = client;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_ptr);
        pthread_detach(tid); // Thread tự giải phóng sau khi xử lý xong

        printf("received from server\n");
        Sleep(1500);
    }
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    pthread_mutex_destroy(&mutex);
    closesocket(server);
    WSACleanup();
    return 0;
}
