#include<stdio.h>
#include<stdarg.h>

// stadarg xử lý các kiểu 
// char, short → được promote lên int
// float → được promote lên double
//... đại diện cho đối số biến đổi
/* 
    - không biết kiểu
    - không biết số lượng
*/
void print(int first,...){
    va_list args;   //con trỏ lưu trạng thái của việc duyệt qua danh sách các đối số
   
    va_start(args,first); 

    printf("%s\n",va_arg(args,char*));
    printf("%.2f\n",va_arg(args,double));
    printf("%s\n",va_arg(args,char*));
    printf("%c\n",(char)va_arg(args,int)); //va_arg(args,char*)

    va_end(args);
}
int main(){
    //các đối số truyền vào lưu dưới dạng 1 chuỗi -> gồm các chuỗi con bên trong đại diện cho từng đối só
    //ví dụ : "\10,"12",\3.14,"hello world",'\y'"
    print(10,"12",3.14,"hello world",'y');
    return 0;
}