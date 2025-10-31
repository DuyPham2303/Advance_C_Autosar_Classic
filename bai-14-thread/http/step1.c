#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
   
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[1024];

    // 1. Khởi tạo Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);   

    // 2. Tạo socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 3. Gán địa chỉ
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("🟢 HTTP Server listening on http://localhost:8080/\n");

    // 4. Vòng lặp nhận kết nối
    while (1) {
        client_fd = accept(server_fd, NULL, NULL);

        //đọc chuỗi request từ client
        int len = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (len <= 0) {
            closesocket(client_fd);
            continue;
        }
        buffer[len] = '\0';

        printf("📩 HTTP Request:\n%s\n", buffer);

        // 5. Kiểm tra phương thức và path
        char method[8], path[256];

        //tách method + path
        sscanf(buffer, "%s %s", method, path);

        if (strcmp(method, "GET") == 0 && strcmp(path, "/") == 0) {
            
            //data thực được gủi
            const char *body = "<html><h1>Hello from C HTTP Server!PHAM CAO DUY</h1></html>";
            char response[1024];
            sprintf(response,
                "HTTP/1.1 200 OK\r\n"           //mã phản hồi 
                "Content-Type: text/html\r\n"   //kiểu nội dung 
                "Content-Length: %d\r\n"        //độ dài mội dung
                "\r\n"
                "%s", (int)strlen(body), body);
            //gửi chuỗi response đúng định dạng HTTP
            send(client_fd, response, strlen(response), 0);
        } else {
            const char *msg = "404 Not Found";
            char response[512];
            sprintf(response,
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: %d\r\n"
                "\r\n"
                "%s", (int)strlen(msg), msg);
            send(client_fd, response, strlen(response), 0);
        }

        closesocket(client_fd);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
