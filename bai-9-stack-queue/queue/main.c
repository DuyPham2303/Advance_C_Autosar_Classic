#include "queue.h"

//n! = n(n -1)(n - 2)...1 -
void giaithua(int n){
    Queue queue;

    Init_Queue(&queue,n);

    for(int i = n ; i > 0 ;i--) enqueue(&queue,i); 

    int result = 1;
    for(int i = 0 ; i < n ;i++){
        result *= getFront(queue);
        printf("%d*",getFront(queue));
        dequeue(&queue);
    }
    printf(" = %d",result); 
}
int main()
{
    Queue queue1;
    int size = 5;
    Init_Queue(&queue1, size);
    giaithua(size);
    return 0;


    /*   for (int i = 0; i < size; i++)
      {
          int data = i + 2;
          enqueue(&queue1, data);
      } */

    enqueue(&queue1, 2);
    enqueue(&queue1, 4);
    enqueue(&queue1, 6);

    dequeue(&queue1);
    dequeue(&queue1);
    dequeue(&queue1);

    dequeue(&queue1);
    /*
        enqueue(&queue1, 7);
        enqueue(&queue1, 8);
        printQueue(queue1);

        printf("front : %d\nrear: %d\n", getFront(queue1), getRear(queue1)); */
}