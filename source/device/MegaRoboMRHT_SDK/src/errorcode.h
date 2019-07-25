#ifndef _MEGA_ERROR_CODE_H_
#define _MEGA_ERROR_CODE_H_

#include "bus.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/**
 * @brief mrgErrorCodeConfigUpload
 * 上传错误代码配置
 * @param vi
 * 网关句柄
 * @param code
 * 错误码
 * @param type
 * 1,2,3 分别对应 错误,警告,信息
 * @param response
 * 1,2,3,4,5,6,7 分别对应 A/B/C/D/E/F/G
 * @param diagnose
 * 0,1是否保存到诊断记录
 * @param enable
 * 是否支持 0,1
 * @return
 */
EXPORT_API int CALL  mrgErrorCodeConfigUpload(ViSession vi, int code, int *type, int *response, int *diagnose, int *enable);
/**
 * @brief mrgErrorCodeConfigDownload
 * 下载错误代码配置
 * @param vi
 * 网关句柄
 * @param code
 * 错误码
 * @param type
 * 1,2,3 分别对应 错误,警告,信息
 * @param response
 * 1,2,3,4,5,6,7 分别对应 A/B/C/D/E/F/G
 * @param diagnose
 * 0,1是否保存到诊断记录
 * @param enable
 * 是否支持 0,1
 * @return
 * 成功返回0, 失败返回负数
 */
EXPORT_API int CALL  mrgErrorCodeConfigDownload(ViSession vi, int code, int type, int response, int diagnose, int enable);
/**
 * @brief mrgErrorLogUpload
 * 获取诊断记录信息
 * @param vi
 * 网关句柄
 * @param errorLog
 * 输出的诊断信息,确保空间足够
 * @return
 * 返回实际的长度
 */
EXPORT_API int CALL mrgErrorLogUpload(ViSession vi, char* errorLog);
/**
 * @brief mrgErrorLogClear
 * 清空诊断记录
 * @param vi
 * 网关句柄
 * @return
 * 0表示成功, -1表示失败
 */
EXPORT_API int CALL mrgErrorLogClear(ViSession vi);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif


