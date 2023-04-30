#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linkedlist.h"
#include "functions.h"

//file for my extra functions

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

void customer_r_log(int customerNo, char serviceType, int hour, int min, int sec) //logs when the customers arrive
{

    FILE *logFp = fopen("r_log","a");
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fprintf(logFp,"%d: %c\n",customerNo,serviceType);
    fprintf(logFp, "Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fclose(logFp);

}

void teller_r_log(FILE *logFp,customerInfo * customer, tellerArgs * data,int hour,int min, int sec){

    fprintf(logFp,"    Teller: %d\n",data->tellerNo);
    fprintf(logFp,"    Customer: %d\n",customer->customerNo);
    fprintf(logFp,"    Arrival time: %02d:%02d:%02d\n", customer->arrivalHour, customer->arrivalMin ,customer->arrivalSec);
    sleep(data->withdrawTime);
    getTime(&hour,&min,&sec);
    fprintf(logFp,"    Completion time: %02d:%02d:%02d\n", hour, min ,sec);
}


void printData(void *data) //used this to check if my customers were inserted into my linked list.
{
    printf("(%d, %c)->", ((customerInfo*)data)->customerNo,((customerInfo*)data)->service);
}

