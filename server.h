#ifndef __SERVER_H__
#define __SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

int initServer(char *ip, int port);
void waitUserInput();

#ifdef __cplusplus
}
#endif

#endif /* __SERVER_H__ */
