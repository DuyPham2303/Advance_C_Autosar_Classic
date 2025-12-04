#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>

//int counter = 0;

_Atomic int counter = 0;  //từ khóa có sẵn trong chuẩn C11

//atomic_int counter = 0;    //typedef được định nghĩa trong <stdatomic.h>

/*
    quá trình thực thi của câu lệnh "counter++" trong CPU:

        register = counter;      // 1.load  -> Đọc giá trị hiện tại (RAM -> Register)
        register = register + 1; // 2.add   -> Tính giá trị mới     (Register -> ALU)
        counter  = register;     // 3.store -> Ghi lại              (ALU -> Register -> RAM)

    Khi chỉ có 1 luồng → thao tác này an toàn.
    Nhưng khi có nhiều luồng cùng truy cập, ví dụ:

        t1: load(100)  → compute(101)
        t2: load(100)  → compute(101)
        t1: store(101)
        t2: store(101) → ghi đè kết quả của t1

    Kết quả cuối cùng chỉ còn 101 thay vì 102 ⇒ mất 1 lần tăng (race condition).
*/
void* increment(void* arg) {
    for (int i = 0; i < 1000000; ++i)
        atomic_fetch_add(&counter,1); 
        //counter++;
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Expected = 2000000, Actual = %d\n", atomic_load(&counter));
}
