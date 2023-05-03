//file for my additional functions
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



void getTime(int *hour, int *min, int * sec) //got a little help from stack overflow to figure out how to use time but i did not copy //Antrromet. 2011. “Get the Current Time in C.” Stack Overflow. February 28, 2011. https://stackoverflow.com/questions/5141960/get-the-current-time-in-c.
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

void customer_r_log(int customerNo, char serviceType, int hour, int min, int sec) //logs when the customers arrive
{

    FILE *logFp = fopen("r_log","a");
    fprintf(logFp,"\n-----------------------------------------------------------------------\n");
    fprintf(logFp,"%d: %c\n",customerNo,serviceType);
    fprintf(logFp, "Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fflush(logFp);  // this helped to instantly push it to the file
    fclose(logFp);

}

void log_completion_time(customerInfo * customer, tellerArgs * data,int time) // logs the time it takes for the customer to be served by teller into rlog
{
    int hour, min, sec;
    FILE *logFp = fopen("r_log", "a");
    fprintf(logFp,"\nTeller: %d\n",data->tellerNo);
    fprintf(logFp,"Customer: %d\n",customer->customerNo);
    fprintf(logFp,"Arrival time: %02d:%02d:%02d\n", customer->arrivalHour, customer->arrivalMin ,customer->arrivalSec);
    sleep(time);
    getTime(&hour,&min,&sec);
    fprintf(logFp,"Completion time: %02d:%02d:%02d\n", hour, min ,sec);
    fflush(logFp);  // this helped to instantly push it to the file
    fclose(logFp);
}

void terminate_r_log(FILE *logFp,customerInfo * customer, tellerArgs * data,int servedCount) //logs the termination of tellers into rlog

{
    int hour, min, sec;
    fprintf(logFp,"\nTermination: Teller-%d\n",data->tellerNo);
    fprintf(logFp,"Number of Served Customer: %d\n",servedCount);
    fprintf(logFp, "Start time: %02d:%02d:%02d\n", data->startHour, data->startMin ,data->startSec);
    getTime(&hour, &min, &sec);
    fprintf(logFp, "Termination time: %02d:%02d:%02d\n", hour, min ,sec);
    fflush(logFp);  // had to use fflush because they were logging their termination in random order so this helped to instantly push it to the file
}


void log_response_time(customerInfo* customer,tellerArgs* data) // logs the tellers response time to the log file
{
    int hour, min, sec;
    FILE *logFp = fopen("r_log", "a");
    fprintf(logFp,"\nTeller: %d\n",data->tellerNo);
    fprintf(logFp,"Customer: %d\n",customer->customerNo);
    getTime(&hour,&min,&sec);
    fprintf(logFp, "Arrival time: %02d:%02d:%02d\n",customer->arrivalHour, customer->arrivalMin ,customer->arrivalSec);
    fprintf(logFp, "Response time: %02d:%02d:%02d\n", hour, min ,sec);
    fflush(logFp);  // this helped to instantly push it to the file
    fclose(logFp);

}