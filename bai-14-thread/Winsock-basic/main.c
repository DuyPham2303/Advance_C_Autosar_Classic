/*
    Khởi tạo Winsock
*/

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Thư viện Winsock

int main(int argc, char *argv[])
{
    WSADATA wsa; // biến lưu thông tin bổ sung

    printf("\nĐang khởi tạo Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // phiên bản sử dụng và biến WSADATA
    {
        printf("Thất bại. Mã lỗi: %d\n", WSAGetLastError());
        return 1;
    }

    printf("Đã khởi tạo Winsock.\n");

    SOCKET bridge;
    /*
        + Họ địa chỉ: AF_INET (đây là phiên bản IP 4)
        + Loại: SOCK_STREAM (điều này có nghĩa là giao thức TCP hướng kết nối)
        + Giao thức: 0 [hoặc IPPROTO_TCP, IPPROTO_UDP]
    */
    //Tạo socket (cầu nối trao đổi data giữa client - server)
    if ((bridge = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        printf("Không thể tạo socket : %d", WSAGetLastError());
        return 1;
    }

    printf("Ổ cắm đã được tạo.\n");

    // Kết nối tới server
    
    struct sockaddr_in server;
    server.sin_addr.S_un.S_addr = inet_addr("93.184.216.34"); // IP của example.com

    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    if(connect(bridge,(struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("lỗi kết nối Server\n");
		return 1;
	}
	
	puts("Đã kết nối Server\n");


    //gửi data
    /* 
        - kết nối đến một địa chỉ IP và sau đó gửi chuỗi tin nhắn 
        - Tin nhắn thực chất là lệnh http để lấy trang chính của một trang web.
    */
    char* message = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";

    if(send(bridge,message,strlen(message), 0) < 0)
	{
		puts("Gửi không thành công\n");
		return 1;
	}
	puts("Gửi dữ liệu\n");

    //nhận data
    int recv_size = 0;
    char server_reply[1024]; 

    //kích thước thực của data received
    recv_size = recv(bridge, server_reply, sizeof(server_reply) - 1, 0);
	
	puts("Đã nhận được phản hồi\n");

	//Thêm ký tự kết thúc NULL để tạo thành chuỗi đúng trước khi in
	server_reply[recv_size] = '\0';
    printf("client received : %s\n",server_reply);

    closesocket(bridge);
    WSACleanup();
/*
    //Note
    /* 
        + Khái niệm "kết nối" áp dụng cho loại socket SOCK_STREAM/TCP. 
        + Kết nối có nghĩa là một "luồng" dữ liệu đáng tin cậy sao cho 
        + có thể có nhiều luồng như vậy, mỗi luồng có giao tiếp riêng. 
        => một đường ống không bị dữ liệu khác can thiệp.
        + Các socket khác như UDP, ICMP, ARP không có khái niệm "kết nối". 
        => giao tiếp không dựa trên kết nối.
        =>  nghĩa là bạn tiếp tục gửi hoặc nhận các gói tin từ bất kỳ ai
    */
    return 0;
}