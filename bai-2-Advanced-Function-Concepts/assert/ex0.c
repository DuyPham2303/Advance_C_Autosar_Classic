#include <stdio.h>
#include <assert.h>

#define LOG_ERROR(error,str) assert(error && #str)

float divide(int mauso,int tuso){
    LOG_ERROR(mauso !=0,mau so phai khac 0);
    return (float)tuso/mauso;
}
int main(){
    int tuso = 2,mauso = 0;
    int result = divide(mauso,tuso);
    printf("result = %.2f",result);
    return 0;
}