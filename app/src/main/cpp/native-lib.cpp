#include <jni.h>
#include <android/native_window_jni.h>
#include "ChaoLog.h"
#include "ChaoPlayerPorxy.h"

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    ChaoPlayerPorxy::Get()->Init(vm);

    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_ChaoPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    ChaoPlayerPorxy::Get()->InitView(win);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_OpenActivity_Open(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    // 打开本地视频
    ChaoPlayerPorxy::Get()->Open(url);
    ChaoPlayerPorxy::Get()->Start();
    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_lichao_chaoplayer_MainActivity_PlayPos(JNIEnv *env, jobject instance) {
    // 播放进度条
    return ChaoPlayerPorxy::Get()->PlayPos();
}