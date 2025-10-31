#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

char* read_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *content = malloc(size + 1);
    fread(content, 1, size, f);
    content[size] = '\0';

    fclose(f);
    return content;
}

void send_response(SOCKET client_fd, const char *status, const char *content_type, const char *body) {
    char response[4096];
    sprintf(response,
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s", status, content_type, (int)strlen(body), body);
    send(client_fd, response, strlen(response), 0);
}

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[1024];

    WSAStartup(MAKEWORD(2, 2), &wsa);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("🟢 Server listening on http://localhost:8080/\n");

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);
        int len = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0) {
            closesocket(client_fd);
            continue;
        }
        buffer[len] = '\0';

        char method[8], path[256];
        sscanf(buffer, "%s %s", method, path);

        if (strcmp(method, "GET") == 0) {
            if (strcmp(path, "/") == 0) {
                char *html = read_file("index1.html");
                if (html) {
                    send_response(client_fd, "200 OK", "text/html", html);
                    free(html);
                } else {
                    send_response(client_fd, "404 Not Found", "text/plain", "Not Found");
                }
            } else if (strcmp(path, "/styles.css") == 0) {
                char *css = read_file("styles.css");
                if (css) {
                    send_response(client_fd, "200 OK", "text/css", css);
                    free(css);
                } else {
                    send_response(client_fd, "404 Not Found", "text/plain", "Not Found");
                }
            } else if (strcmp(path, "/script.js") == 0) {
                char *js = read_file("script.js");
                if (js) {
                    send_response(client_fd, "200 OK", "application/javascript", js);
                    free(js);
                } else {
                    send_response(client_fd, "404 Not Found", "text/plain", "Not Found");
                }
            } else if (strcmp(path, "/data") == 0) {
                char *json = read_file("data.json");  // hoặc csv nếu bạn có
                if (json) {
                    send_response(client_fd, "200 OK", "application/json", json);
                    free(json);
                } else {
                    send_response(client_fd, "500 Internal Server Error", "text/plain", "Can't read data");
                }
            } else {
                send_response(client_fd, "404 Not Found", "text/plain", "Not Found");
            }
        }

        closesocket(client_fd);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
