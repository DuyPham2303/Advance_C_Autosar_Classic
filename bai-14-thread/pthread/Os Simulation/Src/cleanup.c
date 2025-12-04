void cleanup_thread(void* arg) {
    printf("[Cleanup] Thread %s dọn tài nguyên.\n", (char*)arg);
}

void* Task_Com(void* arg) {
    pthread_cleanup_push(cleanup_thread, "Task_Com");

    while (1) {
        WaitEvent(EVENT_TX);
        printf("[Task_Com] Gửi dữ liệu CAN\n");
        ClearEvent(&Task_Com, EVENT_TX);
        sleep(1);
    }

    pthread_cleanup_pop(1);  // Gọi cleanup khi kết thúc
    return NULL;
}
