#include "Swc_ActuatorIf.h"
#include "Rte_Swc_ActuatorIf.h"
#include <stdio.h>

void Swc_ActuatorIf_Init(void)
{
    /* nothing to init */
    printf("[Swc_ActuatorIf] Init\n");
}

/* Đọc ActuatorCmd từ RTE rồi gọi IoHwAb qua CS */
void Swc_ActuatorIf_Run10ms(void)
{
    ActuatorCmd_s a;
    if (Rte_Read_ActuatorCmd(&a) == E_OK) {
        if (a.duty_pct > 100u) a.duty_pct = 100u;   /* clamp */
        (void)Rte_Call_Actuation_SetDirection(a.dir);
        (void)Rte_Call_Actuation_SetPwm(a.duty_pct);
        printf("[Swc_ActuatorIf] APPLY dir=%d duty=%u%%\n",
               (int)a.dir, (unsigned)a.duty_pct);
    } else {
        /* không có lệnh mới → bỏ qua */
    }
}
