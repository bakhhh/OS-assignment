#ifndef FUNCTIONS
#define FUNCTIONS
#include "linkedlist.h"
#include "teller.h"
#include "customer.h"



void getTime(int *hour, int *min, int * sec);
void resetLog();
void customer_r_log(int customerNo, char serviceType, int hour, int min, int sec);
void teller_r_log(FILE *logFp,customerInfo * customer, tellerArgs * data,int hour,int min, int sec);
void terminate_r_log(FILE *logFp,customerInfo * customer, tellerArgs * data,int servedCount,int hour,int min, int sec);
void printData(void *data);

#endif
