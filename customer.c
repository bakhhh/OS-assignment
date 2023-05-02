//file for my customer function
//Name: Sohail Bakhshi
//ID: 20605126
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



void customer(void *arguments)
{
    customerArgs *data = arguments;
    FILE *fpCustomerFile = fopen("c_file", "r"); // open file for reading
    int customerFile;

    while(1){
        pthread_mutex_lock(&tellerMutex); // lock 
        while (data->queue->count == data->queue->queueSize)
        {
            pthread_cond_wait(&tellerCond, &tellerMutex); // make the the customer thread wait if the queue is full
        }
        if (data->queue->count != data->queue->queueSize) 
        {
            pthread_mutex_unlock(&tellerMutex); //when the queue isnt full unlock the mutex and allow the thread to continue adding customers
            customerInfo *customers = (customerInfo*)(malloc(sizeof(customerInfo)));
            customerFile = fscanf(fpCustomerFile, "%d %c\n", &customers->customerNo, &customers->service);

            if (customerFile == EOF) // if it reaches the end of the file then it breaks because theres no more customers to serve
            { 
                free(customers);  // free memory allocated for customers
                terminateTellers = 1; // terminate all tellers
                pthread_cond_broadcast(&tellerCond); //broadcast to all tellers saying that it has reached the end of the file and they should terminate
                break; 
            }
            insertLast(data->queue, customers);
            getTime(&customers->arrivalHour,&customers->arrivalMin,&customers->arrivalSec);
            customer_r_log(customers->customerNo, customers->service,customers->arrivalHour, customers->arrivalMin, customers->arrivalSec);
            pthread_cond_broadcast(&tellerCond); // broadcast to all teller threads so all threads are awakened and not just 1 if i were to use signal
            sleep(data->customerTime); // simulates the time for customers to enter the queue
        }
    }
    fclose(fpCustomerFile);
}
