#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_REQUEST 4096
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
void send_response(SOCKET client, const char *status, const char *type, const char *body) {
    char response[MAX_REQUEST];
    sprintf(response,
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s", status, type, (int)strlen(body), body);
    send(client, response, strlen(response), 0);
}

int get_content_length(const char *request) {
    const char *cl = strstr(request, "Content-Length:");
    if (!cl) return 0;
    int len = 0;
    sscanf(cl, "Content-Length: %d", &len);
    return len;
}

char* get_body(const char *request) {
    const char *body = strstr(request, "\r\n\r\n");
    return body ? (char *)(body + 4) : NULL;
}

void save_data_to_file(const char *data) {
    FILE *f = fopen("saved_data.json", "a");
    if (f) {
        fprintf(f, "%s\n", data);
        fclose(f);
    }
}

int main() {
    WSADATA wsa;
    SOCKET server, client;
    struct sockaddr_in addr;
    char buffer[MAX_REQUEST];

    WSAStartup(MAKEWORD(2, 2), &wsa);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (struct sockaddr *)&addr, sizeof(addr));
    listen(server, 5);
    printf("🟢 Server listening on http://localhost:8080/\n");

    while (1) {
        client = accept(server, NULL, NULL);
        int len = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0) {
            closesocket(client);
            continue;
        }
        buffer[len] = '\0';

        char method[8], path[256];
        sscanf(buffer, "%s %s", method, path);

        if (strcmp(method, "POST") == 0 && strcmp(path, "/save-data") == 0) {
            int content_length = get_content_length(buffer);
            char *body = get_body(buffer);

            // Nếu body chưa đủ, đọc tiếp
            int body_len = strlen(body);
            while (body_len < content_length) {
                int r = recv(client, buffer + len, sizeof(buffer) - len - 1, 0);
                if (r <= 0) break;
                len += r;
                buffer[len] = '\0';
                body = get_body(buffer);
                body_len = strlen(body);
            }

            if (body && strlen(body) > 0) {
                save_data_to_file(body);
                send_response(client, "200 OK", "text/plain", "Saved");
            } else {
                send_response(client, "400 Bad Request", "text/plain", "No body received");
            }
        }
        else if (strcmp(method, "GET") == 0 && strcmp(path, "/") == 0) {
            char *html = read_file("post_form.html");
            if (html) {
                send_response(client, "200 OK", "text/html", html);
                free(html);
            }
            else {
                send_response(client, "404 Not Found", "text/plain", "Không tìm thấy index.html");
            }
        }
        else {
            send_response(client, "404 Not Found", "text/plain", "Not Found");
        }

        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
