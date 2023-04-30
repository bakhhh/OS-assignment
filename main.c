#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linkedlist.h"
#include "functions.h"


pthread_mutex_t tellerMutex;
pthread_cond_t tellerCond;

int terminateTellers= 0;
int activeTellers= 4;
int tellerServedCount[4] = {0};
int totalServed=0;


void customer(void *arg){
    customerArgs *data = arg;
    int hour, min, sec;
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
            insertLast(data->queue, customers);
            getTime(&hour,&min,&sec);
            r_log(customers->customerNo, customers->service, hour, min, sec);
            pthread_cond_signal(&tellerCond);
            pthread_mutex_unlock(&tellerMutex);
            sleep(data->customerTime);
        }
    }
    fclose(fpFile);
}



void teller(void *arg){
    tellerArgs * data = arg;
    customerInfo * customer;
    int hour, min, sec;
    int servedCount=0;

    pthread_mutex_lock(&tellerMutex);
    
    while(1){
        if (terminateTellers==1 && data->queue->count ==0){
            totalServed+= servedCount;

            
            printf("Terminating teller %d\n",data->tellerNo);
            pthread_cond_broadcast(&tellerCond);
            pthread_mutex_unlock(&tellerMutex);

            pthread_mutex_lock(&tellerMutex);
            FILE *logFp = fopen("r_log","a");
            fprintf(logFp,"\nTermination: Teller-%d\n",data->tellerNo);
            fprintf(logFp,"Number of Served Customer: %d\n",servedCount);
            fprintf(logFp, "Start time: %02d:%02d:%02d\n", data->startHour, data->startMin ,data->startSec);
            getTime(&hour, &min, &sec);
            fprintf(logFp, "Termination time: %02d:%02d:%02d\n", hour, min ,sec);
            activeTellers --;
            pthread_mutex_unlock(&tellerMutex);

            if (activeTellers == 0){ // last teller returns all teller stats to the file
                // printf("last teller %d\n",data->tellerNo );
                fprintf(logFp,"\nTeller Statistics\n");
                for (int i = 0; i < 4; i++) {
                    fprintf(logFp, "Teller-%d Serves %d customers\n", i+1, tellerServedCount[i]);
                }
                fprintf(logFp,"\nTotal number of customers: %d customers. \n",totalServed);

            }

            break;
        }
    
        while(data->queue->count ==0 && terminateTellers ==0){
            pthread_cond_wait(&tellerCond, &tellerMutex);
        }

        if(data->queue->count >0){  
            customer = deleteFirst(data->queue); // remove customer from queue
            // printf("teller %d in critical section\n",data->tellerNo);
            servedCount ++;
            tellerServedCount[data->tellerNo-1]++;
            pthread_mutex_unlock(&tellerMutex);
            pthread_cond_signal(&tellerCond);
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
                sleep(data->withdrawTime);
                getTime(&hour,&min,&sec);
                fprintf(logFp,"    Completion time: %02d:%02d:%02d\n", hour, min ,sec);

            }
            else if (strcmp(&customer->service,"I")==0){
                fprintf(logFp,"    Teller: %d\n",data->tellerNo);
                fprintf(logFp,"    Customer: %d\n",customer->customerNo);
                fprintf(logFp,"    Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
                sleep(data->informationTime);
                getTime(&hour,&min,&sec);
                fprintf(logFp,"    Completion time: %02d:%02d:%02d\n", hour, min ,sec);

            }
            else if (strcmp(&customer->service,"D")==0){
                fprintf(logFp,"    Teller: %d\n",data->tellerNo);
                fprintf(logFp,"    Customer: %d\n",customer->customerNo);
                fprintf(logFp,"    Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
                sleep(data->depositTime);
                getTime(&hour,&min,&sec);
                fprintf(logFp,"    Completion time: %02d:%02d:%02d\n", hour, min ,sec);

            }
            // printf("teller %d done from critical section\n",data->tellerNo);
            fclose(logFp);
            pthread_mutex_lock(&tellerMutex); // was getting a race condition where mutliple threads where freeing memory that had already been freed.
            free(customer);
            pthread_mutex_unlock(&tellerMutex);
        
            }
    }
}

int main(int argc, char *argv[]){
    if (argc != 6){
        printf("----------------------------\nError: To run enter ./main m tc tw td ti\nQueue Size (m)\nTime for customers to enter queue (tc)\nTime to withdraw (tw)\nTime to deposit(td)\nTime to get information(ti)\n----------------------------\n");
    }
    else{

        
        int queueSize = atoi(argv[1]);
        int tc = atoi(argv[2]); 
        int tw = atoi(argv[3]);
        int td = atoi(argv[4]);
        int ti = atoi(argv[5]);

        int startHour, startMin, startSec;
        resetLog(); //reset the log file each time i run the program
        pthread_t customerthread;
        pthread_t teller1;
        pthread_t teller2;
        pthread_t teller3;
        pthread_t teller4;
        linkedlist * queue = createLinkedList(queueSize);
        customerArgs data = {queue,tc};
        getTime(&startHour, &startMin, &startSec);

        tellerArgs teller1Args = {queue, 1 ,startHour,startMin,startSec,tw,td,ti};
        tellerArgs teller2Args = {queue, 2,startHour,startMin,startSec,tw,td,ti};
        tellerArgs teller3Args = {queue, 3,startHour,startMin,startSec,tw,td,ti};
        tellerArgs teller4Args = {queue, 4,startHour,startMin,startSec,tw,td,ti};
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

    }



    return 0;
}