#include <stdio.h>

void main()
{
    FILE *file = fopen("database.csv", "r");

    if (file == NULL)
    {
        perror("MỞ FILE THẤT BẠI");
        return;
    }

    //ghi file 
    // fprintf(file, "Họ và tên, Tuổi, Địa chỉ, Số điện thoại\n");
    // fprintf(file, "John Doe, 30, 123 Main St, 555-1234\n");
    // fprintf(file, "Jane Smith, 25, 456 Oak St, 555-5678\n");
    // fprintf(file, "Bob Johnson, 40, 789 Pine St, 555-8765\n");

    char buff[50];
    
    //đọc từng dòng
    while(fgets(buff,sizeof(buff),file)){
        printf("%s",buff);
    }


    fclose(file);
}