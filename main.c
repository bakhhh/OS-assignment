#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "linkedlist.h"

pthread_t customerthread;
pthread_t teller1;
pthread_t teller2;
pthread_t teller3;
pthread_t teller4;
pthread_mutex_t tellerMutex;
pthread_cond_t tellerCond;

void r_log(int customerNo, char serviceType, int hour, int min, int sec){

    FILE *logFp = fopen("r_log","a");
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fprintf(logFp,"%d: %c\n",customerNo,serviceType);
    fprintf(logFp, "Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fclose(logFp);

}


void getTime(int *hour, int *min, int * sec) //https://stackoverflow.com/questions/5141960/get-the-current-time-in-c a cheeky little help from stack overflow
{
    time_t timeNow;
    struct tm * timeinfo;
    time(&timeNow);
    timeinfo = localtime(&timeNow);
    *hour =timeinfo->tm_hour;
    *min = timeinfo->tm_min;
    *sec = timeinfo->tm_sec;


}

typedef struct customerArgs{
    linkedlist * queue;
    int tc;
    int size;
}customerArgs;

typedef struct tellerArgs{
    linkedlist * queue;
    char * tellerName;
    int queueSize;

}tellerArgs;

typedef struct customerInfo{
    int customerNo;
    char service;

}customerInfo;

void * customer(void *arg){
    customerArgs *data = arg;
    int hour,min,sec;
    FILE *fpFile = fopen("c_file", "r");
    for (int i =0; i<data->size;i++){
        customerInfo *customers = (customerInfo*)(malloc(sizeof(customerInfo)));
        sleep(data->tc);
        int file_items = fscanf(fpFile, "%d %c\n", &customers->customerNo, &customers->service);
        if (file_items != 2) {
            free(customers);  // free memory allocated for customers
            break; 
        }
        insertLast(data->queue, customers);
        getTime(&hour, &min, &sec);
        r_log(customers->customerNo, customers->service, hour, min, sec);
        pthread_cond_broadcast(&tellerCond);
    }
    fclose(fpFile);
  
    return NULL;
}

void printData(void *data){
    printf("(%d, %c)->", ((customerInfo*)data)->customerNo,((customerInfo*)data)->service);
}

void teller(void *arg){
    tellerArgs * data = arg;
    customerInfo * customer;
    pthread_mutex_lock(&tellerMutex);
    while(1){
    
        while(data->queue->count ==0){
            // printf("Waiting for customer...\n");
            pthread_cond_wait(&tellerCond, &tellerMutex);
        }

        while(data->queue->count == data->queueSize){
            printf("Queue is full...\n");
            pthread_cond_wait(&tellerCond, &tellerMutex);
        }

        if(data->queue->count >0){          
            customer = deleteFirst(data->queue);
            pthread_mutex_unlock(&tellerMutex);
            printf("%s Serving: Customer %d\n",data->tellerName,customer->customerNo);
        
            }
        else{
            pthread_mutex_unlock(&tellerMutex);
        }
    }


}



int main(int argc, char *argv[]){
    int tc =1;
    int queueSize = 10;
    linkedlist * queue = createLinkedList();
    customerArgs data = {queue,tc,queueSize};
    tellerArgs teller1Args = {queue, "Teller 1",queueSize};
    tellerArgs teller2Args = {queue, "Teller 2",queueSize};
    tellerArgs teller3Args = {queue, "Teller 3",queueSize};
    tellerArgs teller4Args = {queue, "Teller 4",queueSize};
    pthread_mutex_init(&tellerMutex, NULL);
    pthread_cond_init(&tellerCond, NULL);
    

    pthread_create(&customerthread,NULL,(void*)customer,(void*)&data);
    pthread_create(&teller1,NULL,(void*)teller,(void*)&teller1Args);
    pthread_create(&teller2,NULL,(void*)teller,(void*)&teller2Args);
    pthread_create(&teller3,NULL,(void*)teller,(void*)&teller3Args);
    pthread_create(&teller4,NULL,(void*)teller,(void*)&teller4Args);
    pthread_join(customerthread,NULL);

    pthread_join(teller1,NULL);
    pthread_join(teller2,NULL);
    pthread_join(teller3,NULL);
    pthread_join(teller4,NULL);

    freeLinkedList(queue);
    pthread_mutex_destroy(&tellerMutex);
    pthread_cond_destroy(&tellerCond);











    return 0;
}