#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void r_log(int customerNo, char serviceType, int hour, int min, int sec){

    FILE *logFp = fopen("r_log","a");
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fprintf(logFp,"%d: %c\n",customerNo,serviceType);
    fprintf(logFp, "Arrival time: %02d:%02d:%02d\n", hour, min ,sec);
    fprintf(logFp,"-----------------------------------------------------------------------\n");
    fclose(logFp);

}

void getTime(int *hour, int *min, int * sec) //https://stackoverflow.com/questions/5141960/get-the-current-time-in-c
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    *hour =timeinfo->tm_hour;
    *min = timeinfo->tm_min;
    *sec = timeinfo->tm_sec;


}

void enqueue() {
    FILE *fpFile = fopen("c_file", "r");
    FILE *fp = fopen("c_queue", "w");
    char line[20];
    int customerNo;
    char serviceType;
    int hour,min,sec;

    
    for (int i =0; i<20;i++){
        usleep(500000);
        fgets(line, 20, fpFile);
        fputs(line, fp);
        sscanf(line, "%d %c", &customerNo, &serviceType);
        getTime(&hour,&min,&sec);
        r_log(customerNo,serviceType,hour,min,sec);
    
    }

    fclose(fp);
    fclose(fpFile);
  
}

void * customer(){

    return NULL;
}




int main(int argc, char *argv[]){

    enqueue();










    return 0;
}