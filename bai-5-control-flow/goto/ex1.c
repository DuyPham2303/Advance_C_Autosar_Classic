#include <stdio.h>

int main(){
    while(1){
        for(int i = 0 ; i < 5 ; i++){
            for(int j = 0 ; j < 3 ; j++){
                if(i == 1 && j == 2){
                    printf("break\n");
                    goto exit_loop;
                    //break;
                }
            }
            if(i == 1){
                printf("break\n");
                //break;
            }
        }
        printf("break\n");
        //break;
    }
    exit_loop:
    printf("exit loop");
    return 0;
}