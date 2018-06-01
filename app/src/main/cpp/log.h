//
// Created by CoulsonChen on 2018/5/28.
//

#ifndef SUNGAMEENGINE_LOG_H
#define SUNGAMEENGINE_LOG_H
#include <android/log.h>

#define LOG_TAG "SunGameEngine"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif //SUNGAMEENGINE_LOG_H
