#ifndef TELLER
#define TELLER


pthread_mutex_t tellerMutex;
pthread_cond_t tellerCond;

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

int terminateTellers;



void teller(void *arguments);
 
#endif

