#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

void enqueue(int tc,int queueSize) {
    FILE *fpFile = fopen("c_file", "r");
    FILE *fpQueue = fopen("c_queue", "w");
    char line[20];
    int customerNo;
    char serviceType;
    int hour,min,sec;

    
    for (int i =0; i<queueSize;i++){
        sleep(tc);
        fgets(line, queueSize, fpFile);
        fputs(line, fpQueue);
        sscanf(line, "%d %c", &customerNo, &serviceType);
        getTime(&hour,&min,&sec);
        r_log(customerNo,serviceType,hour,min,sec);
    
    }

    fclose(fpQueue);
    fclose(fpFile);
  
}

typedef struct customerArgs{
    int *tc;
    int size;
}args;

void * customer(void* arg){
    args * data = arg;
    enqueue(*data->tc,data->size);

    return NULL;
}




int main(int argc, char *argv[]){
    int tc =1;
    int queueSize = 5;
    args data = {&tc,queueSize};

    pthread_create(&thread1,NULL,customer,&data);
    pthread_join(thread1,NULL);













    return 0;
}