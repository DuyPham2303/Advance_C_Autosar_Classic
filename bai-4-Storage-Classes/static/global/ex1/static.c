#include"lib.h"
phanso nhan2ps(phanso ps_a,phanso ps_b){
    phanso sum;
    sum.mauso = ps_a.mauso * ps_b.mauso;
    sum.tuso = ps_a.tuso * ps_b.tuso;
    return sum;
}

void print(phanso ps_a,phanso ps_b){
    phanso ketqua = nhan2ps(ps_a,ps_b);
    printf("%d/%d * %d/%d = %d/%d",
                            ps_a.mauso,ps_a.tuso,
                            ps_b.mauso,ps_b.tuso,
                            ketqua.mauso,ketqua.tuso);
}