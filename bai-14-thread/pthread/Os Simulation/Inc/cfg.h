#ifndef CFG_H
#define CFG_H
typedef struct {
    char* name;
    void* (*entry)(void*);
    pthread_t handle;
} TaskConfigType;

#endif