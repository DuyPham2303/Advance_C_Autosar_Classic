#include <stdio.h>
#include <stdint.h>
#define MASK    (1UL << 32)
int main()
{
    /*
        + Note: 1 << n
            - 1 sẽ luôn mặc định là int
            - nếu << 31, chạm tới bit dấu , có thể ra kết quả sai
            - luôn ghi 1u , đảm bảo ép thành unsigned int (ko dấus)
                + 1u :  phù hợp <= 31 bit
                + 1ul : phù hợp >= 32 bit
                + 1ull : phù họp 64 bit
        + Tóm tắt
            - Khi làm việc với dịch bit cần lưu ý 
            - Luôn quy định 1u << n để tránh lỗi khi dịch vào bit dấu
            - Phân biệt có/ko u
                -> 1 << 31 : có thể gây lỗi undefined behaviour (tùy compiler)
                -> 1u << 31 : an toàn -> đảm bảo giá trị nguyên duong
                -> dịch bit nhỏ có thể ko cần u (vẫn nên quy định để nhất quán)
                -> dùng ull khi làm việc với uint64_t 
        */


    //
    unsigned int x = (1u << 31);
    printf("1 << 31  = %u\n", x);
    printf("1u << 31 = %d\n", 1u << 31);
}