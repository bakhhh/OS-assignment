#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linkedlist.h"
#include "functions.h"
#include "teller.h"
#include "customer.h"

void customer(void *arguments){
    customerArgs *data = arguments;
    
    FILE *fpFile = fopen("c_file", "r");

    while(1){
        pthread_mutex_lock(&tellerMutex);
        while (data->queue->count == data->queue->queueSize){
            pthread_cond_wait(&tellerCond, &tellerMutex);
        }
        if (data->queue->count != data->queue->queueSize){
            pthread_mutex_unlock(&tellerMutex);
            customerInfo *customers = (customerInfo*)(malloc(sizeof(customerInfo)));
            int customerFile = fscanf(fpFile, "%d %c\n", &customers->customerNo, &customers->service);
            if (customerFile != 2) { // if it reaches the end of the file then it breaks because theres no more customers to serve
                free(customers);  // free memory allocated for customers
                terminateTellers = 1; // terminate all tellers
                pthread_cond_broadcast(&tellerCond); //broadcast to all tellers saying that it has reached the end of the file and they should terminate
                break; 
            }
            pthread_mutex_lock(&tellerMutex);
            insertLast(data->queue, customers);
            getTime(&customers->arrivalHour,&customers->arrivalMin,&customers->arrivalSec);
            customer_r_log(customers->customerNo, customers->service,customers->arrivalHour, customers->arrivalMin, customers->arrivalSec);
            pthread_mutex_unlock(&tellerMutex);
            pthread_cond_broadcast(&tellerCond);
            sleep(data->customerTime);
        }
    }
    fclose(fpFile);
}
