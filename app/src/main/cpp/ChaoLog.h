//
// Created by Administrator on 2018/5/4 0004.
//

#ifndef CHAOPLAYER_CHAOLOG_H
#define CHAOPLAYER_CHAOLOG_H


class ChaoLog {

};
#ifdef ANDROID
#include <android/log.h>
#define CHAOLOGW(...) __android_log_print(ANDROID_LOG_WARN, "ChaoPlayer", __VA_ARGS__)
#define CHAOLOGE(...) __android_log_print(ANDROID_LOG_ERROR, "ChaoPlayer", __VA_ARGS__)
#define CHAOLOGI(...) __android_log_print(ANDROID_LOG_INFO, "ChaoPlayer", __VA_ARGS__)
#define CHAOLOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "ChaoPlayer", __VA_ARGS__)
#else
#define CHAOLOGW(...) printf("ChaoPlay", __VA_ARGS__)
#define CHAOLOGE(...) printf("ChaoPlay", __VA_ARGS__)
#define CHAOLOGI(...) printf("ChaoPlay", __VA_ARGS__)
#define CHAOLOGD(...) printf("ChaoPlay", __VA_ARGS__)
#endif

#endif //CHAOPLAYER_CHAOLOG_H
