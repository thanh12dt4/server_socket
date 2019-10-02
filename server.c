#include "server.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "writelog.h"

#define INFO_ERROR_INPUT        printf("You entered wrong. Please input again following the options:\n"); \
                                printf("<set or get> <name> <value>(value only use for \"set\")\n");

#define LOG_FILE                "/var/log/syslog"
#define APP_INFO                "socketapp server"
#define WRITE_LOG_SERVER(log)   WRITE_LOG(LOG_FILE, APP_INFO, log)

static int socket_desc;
static struct sockaddr_in client;

int initServer(char *ip, int port)
{
    WRITE_LOG_SERVER("start init");
    if (!ip)
    {
        WRITE_LOG_SERVER("ip is empty");
        return 1;
    }

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket\n");
        WRITE_LOG_SERVER("Could not create socket");
        return 1;
    }

    //Prepare the sockaddr_in structure
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(ip);
    client.sin_port = htons(port);
    
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&client , sizeof(client)) < 0)
    {
        puts("connect error\n");
        WRITE_LOG_SERVER("connect error");
        return 1;
    }
    
    puts("Connected\n");
    WRITE_LOG_SERVER("init done");

    return 0;
}

static void sendRequest(char *request, char *reply, int maxReply)
{
    if ((!request) || (!reply))
    {
        WRITE_LOG_SERVER("request or reply is empty");
        return;
    }
    //Send some data
    if(send(socket_desc, request, (strlen(request) + 1), 0) < 0)
    {
        puts("Send failed\n");
        WRITE_LOG_SERVER("send request failed");
        return;
    }
    WRITE_LOG_SERVER("send request done");

    //Receive a reply from the server
    if(recv(socket_desc, reply, maxReply, 0) < 0)
    {
        puts("recv failed\n");
        WRITE_LOG_SERVER("receive reply failed");
    }
}

static void parseReply(char *execute, char *reply)
{
    if ((!execute) || (!reply))
    {
        return;
    }
    
    char log[100] = "reply from client: ";
    strcat(log, reply);
    WRITE_LOG_SERVER(log);

    int argc = 0;
    char *arg;
    char ptr[strlen(reply) + 1];
    char *argv[3];
    
    strcpy(ptr, reply);
    arg = strtok (ptr, " ");
    while ((arg != NULL) || (argc > 3))
    {
        argv[argc] = arg;
        argc++;
        arg = strtok (NULL, " ");
    }
    if (argc > 0)
    {
        switch(strtol(argv[0],NULL,10))
        {
            case 1000:
                printf("%s is OK, name: %s, value: %s\n", execute, argv[1], argv[2]);
                break;
            case 1001:
                printf("%s is failed, name: %s is not exist\n", execute, argv[1]);
                break;
            case 1002:
                printf("%s is failed, can not %s database\n", execute, execute);
                break;
            case 2001:
                printf("%s is failed, error when parsed request at client\n", execute);
                break;
            case 2002:
                printf("%s is failed, error when received request at client\n", execute);
                break;
            default:
                printf("%s is failed, fatal error\n", execute);
                break;
        }   
    }
}

static void parseCommand(char *command)
{
    if (!command)
    {
        printf("Error while parse command\n");
        return;
    }

    int argc = 0;
    char *arg;
    char ptr[strlen(command) + 1];
    char *argv[3];
    
    strcpy(ptr, command);
    arg = strtok (ptr, " ");
    while (arg != NULL)
    {
        argc++;
        if (argc > 3) break;
        argv[argc - 1] = arg;
        
        arg = strtok (NULL, " ");
    }
    
    char request[strlen(argv[0]) + strlen(argv[1]) + strlen(argv[2]) + 10];
    char reply[100];
    switch (argc)
    {
        case 2:
            if (strcmp(argv[0], "get") == 0)
            {
                sprintf(request, "%s %s", argv[0], argv[1]);
                sendRequest(request, reply, 100);
                parseReply("get", reply);
            }
            else
            {
                INFO_ERROR_INPUT
            }
            break;
        case 3:
            if (strcmp(argv[0], "set") == 0)
            {
                sprintf(request, "%s %s %s", argv[0], argv[1], argv[2]);
                sendRequest(request, reply, 100);
                parseReply("set", reply);
            }
            else
            {
                INFO_ERROR_INPUT
            }
            break;
        default:
            INFO_ERROR_INPUT
            break;
    }
}

void waitUserInput()
{
    char command[2000];
    while(1)
    {
        printf("Enter command: ");
        fflush(stdin);
        gets(command);
        parseCommand(command);
    }
}
