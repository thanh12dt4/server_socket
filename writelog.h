#ifndef __WRITE_LOG_H__
#define __WRITE_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WRITE_LOG(logFile, appInfo, log)        writeLog(logFile, appInfo, log)

void writeLog(char *filePath, char *appInfo, char *log);

#ifdef __cplusplus
}
#endif

#endif /* __WRITE_LOG_H__ */
