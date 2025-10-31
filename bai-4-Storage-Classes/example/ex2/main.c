#include "stdio.h"
#include "calculation.h"
int main(){
    double a,b,c,delta;
    double x1,x2;
    while(1){
        //bước 1: nhập các hệ số a,b,c
        input_coefficients(&a,&b,&c);

        //bước 2: tính delta = b^2 - a*a*c
        //delta = calculate_delta(a,b,c);

        //printf("delta = %lf\n",delta);
        
        //bước 3: 
        // - 2 nghiệm phần biệt : x1,x2 delta > 0
        // - nghiệm kép : x1 = x2 = -b/2a delta = 0
        // - vô nghiệm : delta < 0
        //Type result = solve(a,b,c,delta,&x1,&x2);

        display_result(a,b,c,delta);
    }
}