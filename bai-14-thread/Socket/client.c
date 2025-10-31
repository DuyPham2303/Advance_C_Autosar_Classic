#include <stdio.h>
#include <unistd.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    char msg[1024];
    // 1. Khởi tạo Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 2. Tạo socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 3. Cấu hình địa chỉ server
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // 4. Kết nối đến server
    connect(client_socket, (struct sockaddr *)&server, sizeof(server));
    int count = 0;
    while(count < 10){
        // 5. Gửi dữ liệu
        sprintf(msg,"hello server ! count = %d\n",count++);
        send(client_socket, msg, strlen(msg), 0);
        printf("📤 Gửi tới server: %s\t%d\n", msg,count);
        sleep(1);
    }

    // 6. Đóng kết nối
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
