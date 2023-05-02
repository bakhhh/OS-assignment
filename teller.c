//file for my teller function
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

int activeTellers= 4;
int tellerServedCount[4] = {0,0,0,0};
int totalServed=0;

void teller(void *arguments){
    tellerArgs * data = arguments;
    customerInfo * customer;
    int hour, min, sec;
    int servedCount=0;


    while(1)
    {
        pthread_mutex_lock(&tellerMutex); //lock the threads in here 

        while(data->queue->count ==0 && terminateTellers ==0) // if theres no customers in the queue and the tellers havent been told to terminate then the teller threads must wait
        {
            pthread_cond_wait(&tellerCond, &tellerMutex);
        }

        if(data->queue->count >0){  
            customer = deleteFirst(data->queue); // remove customer from queue to be served
            servedCount ++; //count the serve amount
            tellerServedCount[data->tellerNo-1]++;// this will be used later so that the final teller that terminates can output the teller statistics in order with the serve count
            pthread_mutex_unlock(&tellerMutex); // unlock the mutex so the next thread can execute and serve
            pthread_cond_signal(&tellerCond);  // signal to the customer thread that a customer has been served/removed from queue
            //rest of the code below is just inserting into the log file //
            FILE *logFp = fopen("r_log","a");
            fprintf(logFp,"Teller: %d\n",data->tellerNo);
            fprintf(logFp,"Customer: %d\n",customer->customerNo);
            getTime(&hour,&min,&sec);
            fprintf(logFp, "Arrival time: %02d:%02d:%02d\n",customer->arrivalHour, customer->arrivalMin ,customer->arrivalSec);
            fprintf(logFp, "Response time: %02d:%02d:%02d\n", hour, min ,sec);
            printf("Teller %d Serving: Customer %d\n",data->tellerNo,customer->customerNo);
            
            if (strcmp(&customer->service,"W")==0){
                teller_r_log(logFp,customer,data,hour,min,sec);
            }
            else if (strcmp(&customer->service,"I")==0){
                teller_r_log(logFp,customer,data,hour,min,sec);
            }
            else if (strcmp(&customer->service,"D")==0){
                teller_r_log(logFp,customer,data,hour,min,sec);
            }
            fclose(logFp);
            pthread_mutex_lock(&tellerMutex); // used this lock to stop multiple threads from freeing at once which was causing segmentation faults because they were freeing memory that had already been freed.
            free(customer);
            pthread_mutex_unlock(&tellerMutex);//unlock the mutex
            pthread_cond_signal(&tellerCond); // signal to the customer that it has been freed
        
            }
        
        if (terminateTellers==1 && data->queue->count ==0)
        {
            pthread_mutex_unlock(&tellerMutex);//unlock the mutex so all threads are in here
            pthread_mutex_lock(&tellerMutex); //lock the mutex so that 1 thread executes this section of code at a time
            totalServed+= servedCount; 
            printf("Terminating teller %d\n",data->tellerNo);
            FILE *logFp = fopen("r_log","a");
            terminate_r_log(logFp,customer,data,servedCount,hour,min,sec);
            activeTellers --; // done this because each teller is terminating they should subtract from 4 so that the last teller can output the teller statistics
            if (activeTellers == 0){ // last teller returns all teller stats to the file
                fprintf(logFp,"\nTeller Statistics\n");
                for (int i = 0; i < 4; i++) {
                    fprintf(logFp, "Teller-%d Serves %d customers\n", i+1, tellerServedCount[i]);
                }
                fprintf(logFp,"\nTotal number of customers: %d customers. \n",totalServed);
                fclose(logFp);
            }
            pthread_mutex_unlock(&tellerMutex); // unlock so the next thread can execute

            break;
        }
    }
}