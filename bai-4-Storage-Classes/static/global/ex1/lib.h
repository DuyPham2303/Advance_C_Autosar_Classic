#ifndef LIB_H
#define LIB_H
#include "stdio.h"
typedef struct{
    int mauso;
    int tuso;
}phanso;
void print(phanso ps_a,phanso ps_b);
static phanso nhan2ps(phanso ps_a,phanso ps_b);
#endif  