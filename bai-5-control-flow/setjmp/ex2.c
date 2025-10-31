#include <stdio.h>
#include <setjmp.h>
jmp_buf buf;
int exception;

//TRY : KIỂM TRA 1 ĐOẠN CODE CÓ KHẢ NĂNG GÂY LỖI
//CATCH : XÁC ĐỊNH LOẠI LỖI
//THROW : NÉM RA NGOẠI LỆ, IN RA THÔNG BÁO TƯƠNG ỨNG VỚI LỖI 

#define TRY  if((exception = setjmp(buf)) == 0)
#define CATCH(x) else if(exception == x)
#define THROW(x)  longjmp(buf,x);
float divide(int tuso,int mauso){
    if(tuso == 0 && mauso == 0){
        THROW(1);
    }
    else if(mauso == 0){
        THROW(2);
    }
    else{
        return (float)tuso / mauso;
    }
}
int tuso = 0;
int mauso = 0;
int main(){
    while(1){
        printf("nhập tử số:");
        scanf("%d",&tuso);
        printf("nhập mẫu số:");
        scanf("%d",&mauso);
        TRY{
            float result = divide(tuso,mauso);
            printf("\nresult = %.2f\n",result);
        }
        CATCH(1){
            printf("phép toán không tồn tại\n");
        }
        CATCH(2){
            printf("lỗi chia cho 0\n");
        }
    }
    return 0;
}