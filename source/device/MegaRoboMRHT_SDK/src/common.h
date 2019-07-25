#ifndef COMMON_H
#define COMMON_H

char *__GetFileName__(const char *file);

void be_printf(char *fmt, ...);

void print_buffer(char* msg, unsigned char* pBuff, unsigned long ulLen);

#define MRG_LOG(fmt, ...) \
    do{  \
    char tmp[1024] = {0}; \
    snprintf(tmp,sizeof(tmp),"%s(%d):[%s] %s",__GetFileName__(__FILE__),__LINE__,__FUNCTION__, fmt); \
    be_printf(tmp, ##__VA_ARGS__ );  \
    }while(0)

/**
 * @brief strToIntList
 * 拆分字符串到数组
 * @param str 输入需要拆分的字符串
 * @param split 分隔符
 * @param output 输出结果
 * @return 数量
 */
int splitStringToIntArray(const char *str, const char *split, int *output);
/**
 * @brief splitStringToFloatArray
 * 拆分字符串到数组
 * @param str 输入需要拆分的字符串
 * @param split 分隔符
 * @param output 输出结果
 * @return 数量
 */
int splitStringToFloatArray(const char *str, const char *split, float *output);
/**
 * @brief splitString
 * 拆分字符串到数组
 * @param str 输入需要拆分的字符串
 * @param split 分隔符
 * @param output 输出结果
 * @return 数量
 */
int splitString(const char *str, const char *split, char output[][64]);

#endif // COMMON_H
