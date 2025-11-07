#include <assert.h>
#include <stdio.h>

#define LOG_ERROR(error,str) assert(error && #str);

float chia2so(int tuso,int mauso){
    LOG_ERROR(mauso != 0,mauso phai khac 0)
    return (float)tuso/mauso;
}
int main(){
    int tuso = 5,mauso = 2;
    // if(a != 5){
    //     printf("a phai bang 5");
    // }
    // else{

    // }
    //assert(a != 5 && "a phai khac 5");

    float result = chia2so(tuso,mauso);
    printf("a = %f",result);
    return 0;
}