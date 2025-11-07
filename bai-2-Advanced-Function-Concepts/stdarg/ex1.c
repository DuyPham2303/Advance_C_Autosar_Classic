#include <stdio.h>
#include <stdarg.h>

#define SUM(...) sum1(__VA_ARGS__,"abs")

int sum1(int count, ...)
{
  va_list args;
  va_start(args, count);
  
  int result = count;
  
  char* value;

  while ((value = va_arg(args,char*)) != "abs")
  {
    result += (char)value;  //1 - "\1" -> '1' -> 48
  }

  va_end(args); 

  return result;
}
int main()
{
  int result = SUM(12,32,0,124,41);
  printf("result = %d",result);
  return 0;
}
