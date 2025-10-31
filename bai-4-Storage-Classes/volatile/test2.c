#include <stdio.h>
int fakeReg = 0;
int tempsensor = 0;
int main(){
    int*p = &fakeReg;
    while(1){
        //static int count = 0;
        tempsensor = *p;
        if(tempsensor != 0){
            break;
        }
        //count++;
        //if(count == 10) *p = 23;
    }
    printf("tempsensor : %d",tempsensor);
    return 0;
}