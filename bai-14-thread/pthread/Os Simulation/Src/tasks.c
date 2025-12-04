TASK(Task_Com) {
    while (1) {
        WaitEvent(EVENT_TX);
        printf("[Task_Com] Gửi dữ liệu CAN.\n");
        ClearEvent(&Task_Com, EVENT_TX);
    }
    return NULL;
}
