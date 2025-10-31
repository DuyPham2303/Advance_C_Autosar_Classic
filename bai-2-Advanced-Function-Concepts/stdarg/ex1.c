#include <stdio.h>
#include <stdarg.h>

#define SUM(...) sum1(__VA_ARGS__,0)

int sum1(int count, ...)
{
  va_list args;
  va_start(args, count); // xac dinh vi tri bat dau cua danh sach doi so truyen vao
  
  int result = count;
  
  int value = 0;

  while ((value = va_arg(args, int)) != 0)
  {
    result += value; 
  }

  va_end(args); // giai phong con tro

  return result;
}
int main()
{
  int result = SUM(12,32,124,41);
  printf("result = %d",result);
  return 0;
}
