#include "common.h"
#include "platform.h"
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char *__GetFileName__(const char *file)
{
    char *name = NULL;
    char *ptr = (char*)file;
    if(file == NULL)
        return NULL;

    while(*ptr != '\0')
    {
#ifdef __WIN32
        if(*ptr == '\\')
#else
        if(*ptr == '/')
#endif
        {
            name = ptr;
        }
        ptr++;
    }
    return name == NULL ? (char*)file : (name+1);
}

#ifndef MRG_DEBUG
void be_printf(char *fmt, ...){ (void)fmt; }
void print_buffer(char* msg, unsigned char* pBuff, unsigned long ulLen){(void)msg;(void)pBuff;(void)ulLen;}
#else
void be_printf (char *str, ...)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    va_list	vl;
    char	buf[1024] = "";
    va_start (vl, str);
    vsnprintf (buf, sizeof (buf), str, vl);
    va_end (vl);
    buf[sizeof (buf) - 1] = '\0';
    fprintf(stdout, "%s", buf);
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
    return;
}

void print_buffer(char* msg, unsigned char* pBuff, unsigned long ulLen)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    unsigned long ulTmp = 0;
    be_printf("%s:\n", msg);
    be_printf("    ");
    while(ulTmp < ulLen)
    {
        be_printf("%02x ",pBuff[ulTmp]);
        ulTmp++;
        if(!(ulTmp%16))
        {
            be_printf("\n");
            be_printf("    ");
        }
    }
    be_printf("\n");
    pthread_mutex_unlock(&mutex);
}
#endif

int splitStringToIntArray(const char *str, const char *split, int *output)
{
    char *p = NULL;
    char *pNext = NULL;
    int count = 0;
    long val = 0;
    char *endptr = NULL;
    char strBuff[4096] = "";
    if(strlen(str) > sizeof(strBuff))
        return -1;

    strcpy(strBuff, str);
    p = STRTOK_S(strBuff, split, &pNext);
    while (p)
    {
        errno = 0;
        val = strtol(p, &endptr, 10);
        if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
                || (errno != 0 && val == 0))
        {}
        else if (endptr == p)
        {}
        else if (*endptr != '\0')
        {}
        else
        {
            output[count++] = val;
        }
        p = STRTOK_S(NULL, split, &pNext);
    }
    return count;
}

int splitStringToFloatArray(const char *str, const char *split, float *output)
{
    char *p = NULL;
    char *pNext = NULL;
    int count = 0;
    float val = 0;
    char *endptr = NULL;
    char strBuff[4096] = "";
    if(strlen(str) > sizeof(strBuff))
        return -1;

    strcpy(strBuff, str);
    p = STRTOK_S(strBuff, split, &pNext);
    while (p)
    {
        errno = 0;
        val = strtof(p, &endptr);
        if ((errno == ERANGE && val == HUGE_VALF)
                || (errno != 0 && val == 0))
        {}
        else if (endptr == p)
        {}
        else if (*endptr != '\0')
        {}
        else
        {
            output[count++] = val;
        }
        p = STRTOK_S(NULL, split, &pNext);
    }
    return count;
}

int splitString(const char *str, const char *split, char output[][64])
{
    char *p = NULL;
    char *pNext = NULL;
    int count = 0;
    char strBuff[4096] = "";
    if(strlen(str) > sizeof(strBuff))
        return -1;
    strcpy(strBuff, str);
    p = STRTOK_S(strBuff, split, &pNext);
    while (p)
    {
        strcpy( output[count++], p);
        p = STRTOK_S(NULL, split, &pNext);
    }
    return count;
}
