#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linkedlist.h"


pthread_mutex_t tellerMutex;
pthread_cond_t tellerCond;
// int hour, min, sec;
// int startHour, startMin, startSec;
int tw,td,ti;
int terminateTellers= 0;


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

void resetLog() // resets the log file every time i run the program because it was annoying constantly manually deleting it
{
    FILE *logFp = fopen("r_log", "w");
    if (logFp == NULL) {
        printf("Error");
    }
    fclose(logFp);
}

void r_log(int customerNo, char serviceType, int hour, int min, int sec){

    FILE *logFp = fopen("r_log","a");
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fprintf(logFp,"%d: %c\n",customerNo,serviceType);
    fprintf(logFp, "Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fclose(logFp);

}



typedef struct customerArgs{
    linkedlist * queue;
    int tc;
    int size;

}customerArgs;

typedef struct tellerArgs{
    linkedlist * queue;
    int tellerNo;
    int queueSize;
    int startHour;
    int startMin;
    int startSec;

    
}tellerArgs;

typedef struct customerInfo{
    int customerNo;
    char service;

}customerInfo;


void printData(void *data){
    printf("(%d, %c)->", ((customerInfo*)data)->customerNo,((customerInfo*)data)->service);
}

void * customer(void *arg){
    int i;
    customerArgs *data = arg;
    int hour, min, sec;

    FILE *fpFile = fopen("c_file", "r");
    for (i =0; i<data->size;i++){
        customerInfo *customers = (customerInfo*)(malloc(sizeof(customerInfo)));
        sleep(data->tc);
        int customerFile = fscanf(fpFile, "%d %c\n", &customers->customerNo, &customers->service);
        if (customerFile != 2) { // if it reaches the end of the file then it breaks and doesnt add any more
            free(customers);  // free memory allocated for customers
            terminateTellers = 1;
            pthread_cond_broadcast(&tellerCond);
            break; 
        }
        insertLast(data->queue, customers);
        getTime(&hour,&min,&sec);
        r_log(customers->customerNo, customers->service, hour, min, sec);
        pthread_cond_signal(&tellerCond);
    }
    fclose(fpFile);
    
  
    return NULL;
}




void teller(void *arg){
    tellerArgs * data = arg;
    customerInfo * customer;
    int hour, min, sec;

    tw = 1;
    td =1;
    ti =1;

    pthread_mutex_lock(&tellerMutex);
    while(1){
        if (terminateTellers==1 && data->queue->count ==0){
            printf("Terminating teller %d\n",data->tellerNo);
            pthread_cond_broadcast(&tellerCond);
            pthread_mutex_unlock(&tellerMutex);
            FILE *logFp = fopen("r_log","a");
            fprintf(logFp,"------------------------------\n");
            fprintf(logFp,"Termination: Teller-%d\n",data->tellerNo);
            fprintf(logFp,"Served Customer: %d\n",customer->customerNo);
            fprintf(logFp, "Start time: %02d:%02d:%02d\n", data->startHour, data->startMin ,data->startSec);
            getTime(&hour, &min, &sec);
            fprintf(logFp, "Termination time: %02d:%02d:%02d\n", hour, min ,sec);

            break;
        }
    
        while(data->queue->count ==0 && terminateTellers ==0){
         
            printf("teller %d Waiting..\n",data->tellerNo);
            pthread_cond_wait(&tellerCond, &tellerMutex);
    
        }

        // while(data->queue->count == data->queueSize){
        //     printf("Queue is full...\n");
        //     pthread_cond_wait(&tellerCond, &tellerMutex);
        // }

        if(data->queue->count >0){  

            customer = deleteFirst(data->queue);
            pthread_mutex_unlock(&tellerMutex);
            FILE *logFp = fopen("r_log","a");
            fprintf(logFp,"Teller: %d\n",data->tellerNo);
            fprintf(logFp,"Customer: %d\n",customer->customerNo);
            getTime(&hour,&min,&sec);
            fprintf(logFp, "Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
            fprintf(logFp, "Response time: %02d:%02d:%02d\n", hour, min ,sec);
            printf("Teller %d Serving: Customer %d\n",data->tellerNo,customer->customerNo);
            
            if (strcmp(&customer->service,"W")==0){
                fprintf(logFp,"    Teller: %d\n",data->tellerNo);
                fprintf(logFp,"    Customer: %d\n",customer->customerNo);
                fprintf(logFp,"    Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
                sleep(tw);
                getTime(&hour,&min,&sec);
                fprintf(logFp,"    Completion time: %02d:%02d:%02d\n", hour, min ,sec);

            }
            else if (strcmp(&customer->service,"I")==0){
                fprintf(logFp,"    Teller: %d\n",data->tellerNo);
                fprintf(logFp,"    Customer: %d\n",customer->customerNo);
                fprintf(logFp,"    Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
                sleep(ti);
                getTime(&hour,&min,&sec);
                fprintf(logFp,"    Completion time: %02d:%02d:%02d\n", hour, min ,sec);

            
            }
            else if (strcmp(&customer->service,"D")==0){
                fprintf(logFp,"    Teller: %d\n",data->tellerNo);
                fprintf(logFp,"    Customer: %d\n",customer->customerNo);
                fprintf(logFp,"    Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
                sleep(td);
                getTime(&hour,&min,&sec);
                fprintf(logFp,"    Completion time: %02d:%02d:%02d\n", hour, min ,sec);

            }
            fclose(logFp);
        
            }


    }


}


int main(int argc, char *argv[]){
    int tc =1;
    int queueSize = 25;
    int startHour, startMin, startSec;

    
    resetLog(); //reset the log file each time i run the program
    pthread_t customerthread;
    pthread_t teller1;
    pthread_t teller2;
    pthread_t teller3;
    pthread_t teller4;
    linkedlist * queue = createLinkedList();
    customerArgs data = {queue,tc,queueSize};
    getTime(&startHour, &startMin, &startSec);

    tellerArgs teller1Args = {queue, 1 ,queueSize,startHour,startMin,startSec};
    tellerArgs teller2Args = {queue, 2,queueSize,startHour,startMin,startSec};
    tellerArgs teller3Args = {queue, 3,queueSize,startHour,startMin,startSec};
    tellerArgs teller4Args = {queue, 4,queueSize,startHour,startMin,startSec};
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