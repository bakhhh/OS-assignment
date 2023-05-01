//Name: Sohail Bakhshi
//ID: 20605126
// 2023 OS Assignment
// main.c contains the main function which contains threads running the customer and teller functions
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




int main(int argc, char *argv[]){
    int queueSize = atoi(argv[1]); // first argument in the commandline will be the queue size
    int tc = atoi(argv[2]); // second command line argument will be time the customer takes to enter the queue
    int tw = atoi(argv[3]); // third command line argument will be the time it takes for a teller to serve customers that are withdrawing
    int td = atoi(argv[4]); // fourth command line argument will be the time it takes for a teller to serve customers that are depositing
    int ti = atoi(argv[5]); // fifth command line argument will be the time it takes for a teller to serve customers that are wanting information
    int startHour, startMin, startSec; 
    if (argc != 6){
        printf("----------------------------\nError: To run enter ./main m tc tw td ti\nQueue Size (m)\nTime for customers to enter queue (tc)\nTime to withdraw (tw)\nTime to deposit(td)\nTime to get information(ti)\n----------------------------\n");
    }
    else{

        resetLog(); //reset the log file each time i run the program
        pthread_t customerThread; //customer thread
        pthread_t teller1; //teller thread
        pthread_t teller2; //teller thread
        pthread_t teller3; //teller thread
        pthread_t teller4; //teller thread
        linkedlist * queue = createLinkedList(queueSize); //intialising fifo linked list / queue that ill be using to insert customers
        
        customerArgs data = {queue,tc}; // parameters for my customer function used in a struct to pass to customer thread

        getTime(&startHour, &startMin, &startSec);//get the start time of the tellers
        // parameters for my teller function used in a struct to pass to teller thread
        tellerArgs teller1Args = {queue, 1 ,startHour,startMin,startSec,tw,td,ti};
        tellerArgs teller2Args = {queue, 2,startHour,startMin,startSec,tw,td,ti};
        tellerArgs teller3Args = {queue, 3,startHour,startMin,startSec,tw,td,ti};
        tellerArgs teller4Args = {queue, 4,startHour,startMin,startSec,tw,td,ti};

        pthread_mutex_init(&tellerMutex, NULL); // mutex initialised
        pthread_cond_init(&tellerCond, NULL); // condition initialised
        
        //create the threads
        pthread_create(&customerThread,NULL,(void*)customer,(void*)&data);
        pthread_create(&teller1,NULL,(void*)teller,(void*)&teller1Args);
        pthread_create(&teller2,NULL,(void*)teller,(void*)&teller2Args);
        pthread_create(&teller3,NULL,(void*)teller,(void*)&teller3Args);
        pthread_create(&teller4,NULL,(void*)teller,(void*)&teller4Args);
        
        //join the threads
        pthread_join(customerThread,NULL);
        pthread_join(teller1,NULL);
        pthread_join(teller2,NULL);
        pthread_join(teller3,NULL);
        pthread_join(teller4,NULL);

        freeLinkedList(queue); //free allocated memory for the queue
        pthread_mutex_destroy(&tellerMutex); // destroy mutex
        pthread_cond_destroy(&tellerCond); // destroy condition

    }



    return 0;
}