#ifndef CUSTOMER
#define CUSTOMER


pthread_cond_t customerCond;

typedef struct customerArgs{
    linkedlist * queue;
    int customerTime;

}customerArgs;



typedef struct customerInfo{
    int customerNo;
    char service;
    int arrivalHour;
    int arrivalMin;
    int arrivalSec;

}customerInfo;

void customer(void *arguments);

#endif

