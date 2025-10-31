#include<assert.h>
#include<stdio.h>

#define LOG(condition,cmd) assert(condition && #cmd)

#define LOG_IN_RANGE(val,min,max) assert((val) >= (min) && (val) <= (max))

double divide(int mau,int tu){
    LOG(mau != 0,mau phai khac 0);
    return (double)tu/mau;
}

int main(){

    int a = 6;

   /*  if(a == 5) return 0;
    else{
        printf("a phai bang 5");
    } */
    //LOG(a == 5,a phai bang 5);

    double result = divide(12,23);
    printf("result = %.2f",result);

    //LOG_IN_RANGE(12,5,10);

    return 0;
}