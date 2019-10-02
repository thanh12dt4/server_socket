#include <stdio.h>
#include <string.h>
#include <time.h>
#include "writelog.h"

void writeLog(char *filePath, char *appInfo, char *log)
{
    if (filePath == NULL)
    {
        puts("Error when write log: file path is empty");
        return;
    }

    if (appInfo == NULL)
    {
        puts("Error when write log: app info is empty");
        return;
    }

    if (log == NULL)
    {
        puts("Error when write log: Log content is empty");
        return;
    }

    FILE *file;
    file = fopen(filePath, "a");
    if(file == NULL)
    {
      puts("Can not open log file");
    }

    time_t curTime;
    time(&curTime);
    fprintf(file,"%s %s:", strtok(ctime(&curTime), "\n"), appInfo);
    fprintf(file,"%s\n", log);
    fclose(file);
}
