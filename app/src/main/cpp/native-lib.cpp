#include <jni.h>
#include <android/native_window_jni.h>
#include "ChaoLog.h"
#include "ChaoPlayerPorxy.h"

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    // Java vm 接口
    ChaoPlayerPorxy::Get()->Init(vm);
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_util_MyNative_InitView(JNIEnv *env, jobject instance, jobject surface) {
    //初始化窗口
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    ChaoPlayerPorxy::Get()->InitView(win);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_util_MyNative_Open(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    // 打本地视频或网络视频
    ChaoPlayerPorxy::Get()->Open(url);
    ChaoPlayerPorxy::Get()->Start();

    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_lichao_chaoplayer_util_MyNative_PlayPos(JNIEnv *env, jobject instance) {
    // 播放进度条
    return ChaoPlayerPorxy::Get()->PlayPos();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_util_MyNative_Seek(JNIEnv *env, jobject instance, jdouble pos) {
    // 进度条Seek
    ChaoPlayerPorxy::Get()->Seek(pos);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_util_MyNative_PlayOrPause(JNIEnv *env, jobject instance) {
    // 暂停
    ChaoPlayerPorxy::Get()->SetPause(!ChaoPlayerPorxy::Get()->IsPause());
}

