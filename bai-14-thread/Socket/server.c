#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <unistd.h>
#pragma comment(lib, "Ws2_32.lib") // Link với Winsock
void countCheck(int *count, const char *str);
int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_size;
    char buffer[1024];

    // 1. Khởi tạo Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 2. Tạo socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        printf("Không thể tạo socket.\n");
        return 1;
    }

    // 3. Cấu hình địa chỉ server
    server.sin_family = AF_INET;
    server.sin_port = htons(8888); // Cổng 8888
    server.sin_addr.s_addr = INADDR_ANY;

    // 4. Gán socket vào địa chỉ
    bind(server_socket, (struct sockaddr *)&server, sizeof(server));

    // 5. Lắng nghe kết nối
    listen(server_socket, 1);
    printf("🟢 Server đang chạy trên cổng 8888...\n");

    // 6. Chấp nhận kết nối từ client
    client_size = sizeof(client);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &client_size);
    int bytes = 0;
    while (1)
    {
        // 7. Nhận dữ liệu từ client
        bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        static int count = 0;
        buffer[bytes] = '\0';
        printf("📩 Nhận từ client: %s\n", buffer);
        countCheck(&count, (const char *)buffer);
        if (count == 9){
            break;
        }
    }

    // 8. Đóng kết nối
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
void countCheck(int *count, const char *str)
{
    int index = 0;
    while (1)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            *count = str[index] - '0';
            break;
        }
        index++;
    }
}
