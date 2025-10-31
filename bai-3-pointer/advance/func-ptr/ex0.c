#include <stdio.h>
void test1(char* str){
    printf("%s",str);
}

void test2(char* str){
    printf("%s",str);
}
int main()
{
    //<return_type> (*func_pointer)(<data_type_1>, <data_type_2>);
    void  (*func_pointer)(char*) = NULL;

    func_pointer = test1;
    func_pointer("this is test 1\n");

    func_pointer = test2;
    (*func_pointer)("this is test 2\n");

}

