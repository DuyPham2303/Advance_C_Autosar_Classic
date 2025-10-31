#include <stdio.h>
#include <time.h>

//stm32f103 : 32 thanh ghi
//clock 72 MHz : 72,000,000 
int main()
{
   // Lưu thời điểm bắt đầu
   clock_t start_time = clock();
   register int i; 

   // Đoạn mã của chương trình
   for (i = 0; i < 2000000; ++i){}

   // Lưu thời điểm kết thúc
   clock_t end_time = clock();

   // Tính thời gian chạy bằng miligiây
   double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

   printf("Thoi gian chay cua chuong trinh: %f giay\n", time_taken);
   return 0;
}

