#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "linkedlist.h"

pthread_t thread1;


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
    int tc;
    int size;
}args;

typedef struct customerInfo{
    int customerNo;
    char service;

}customerInfo;

void * customer(void *arg){
    linkedlist * queue = createLinkedList();
    args *data = arg;
    int hour,min,sec;
    FILE *fpFile = fopen("c_file", "r");
    for (int i =0; i<data->size;i++){
        customerInfo *customers = (customerInfo*)(malloc(sizeof(customerInfo)));
        sleep(data->tc);
        fscanf(fpFile, "%d %c\n", &customers->customerNo, &customers->service);
        insertLast(queue,customers);
        getTime(&hour,&min,&sec);
        r_log(customers->customerNo,customers->service,hour,min,sec);
    }
    fclose(fpFile);
    return NULL;
}

void printData(void *data){
    printf("(%d, %c)->", ((customerInfo*)data)->customerNo,((customerInfo*)data)->service);
}

int main(int argc, char *argv[]){
    // customer(1,5);
    // printList(queue,&printData);
    int tc =1;
    int queueSize = 5;
    args data = {tc,queueSize};

    pthread_create(&thread1,NULL,(void*)customer,(void*)&data);
    pthread_join(thread1,NULL);













    return 0;
}