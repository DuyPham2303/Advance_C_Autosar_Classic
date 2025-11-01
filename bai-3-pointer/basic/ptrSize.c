#include <stdio.h>
#include <stdbool.h>
int main(){
    printf("%d bytes\n",sizeof(char*));
    printf("%d bytes\n",sizeof(int*));
    printf("%d bytes\n",sizeof(short*));
    printf("%d bytes\n",sizeof(float*));
    printf("%d bytes\n",sizeof(double*));
    printf("%d bytes\n",sizeof(long*));
    printf("%d bytes\n",sizeof(long long*));
    printf("%d bytes\n",sizeof(bool*));
    return 0;
}

