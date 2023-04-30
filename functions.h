#ifndef FUNCTIONS
#define FUNCTIONS
#include "linkedlist.h"



typedef struct customerArgs{
    linkedlist * queue;
    int customerTime;

}customerArgs;

typedef struct tellerArgs{
    linkedlist * queue;
    int tellerNo;
    int startHour;
    int startMin;
    int startSec;
    int withdrawTime;
    int depositTime;
    int informationTime;

    
}tellerArgs;

typedef struct customerInfo{
    int customerNo;
    char service;

}customerInfo;

void getTime(int *hour, int *min, int * sec);
void resetLog();
void r_log(int customerNo, char serviceType, int hour, int min, int sec);
void printData(void *data);

#endif
