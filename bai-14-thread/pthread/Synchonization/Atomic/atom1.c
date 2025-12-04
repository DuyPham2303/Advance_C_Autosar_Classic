#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>

typedef struct {
    _Atomic int id;
    _Atomic int counter;
} Device;

int main() {
    Device *d = malloc(sizeof(Device));
    atomic_init(&d->id, 101);
    atomic_init(&d->counter, 0);

    atomic_fetch_add(&d->counter, 5);

    printf("Device %d - counter = %d\n",
           atomic_load(&d->id),
           atomic_load(&d->counter));

    free(d);
    return 0;
}
