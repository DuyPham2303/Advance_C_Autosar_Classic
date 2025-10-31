#include <stdio.h>
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

        if (strcmp(method, "GET") == 0 && strcmp(path, "/") == 0) {
            char *html = read_file("index.html");
            if (html) {
                char response[2048];
                sprintf(response,
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: %d\r\n"
                    "\r\n"
                    "%s", (int)strlen(html), html);
                send(client_fd, response, strlen(response), 0);
                free(html);
            } else {
                const char *msg = "File not found";
                char response[512];
                sprintf(response,
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: %d\r\n"
                    "\r\n"
                    "%s", (int)strlen(msg), msg);
                send(client_fd, response, strlen(response), 0);
            }
        }

        closesocket(client_fd);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
