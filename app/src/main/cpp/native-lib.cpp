#include <jni.h>
#include <android/native_window_jni.h>
#include "ChaoLog.h"
#include "ChaoPlayerPorxy.h"

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    ChaoPlayerPorxy::Get()->Init(vm);
    ChaoPlayerPorxy::Get()->Open("/sdcard/Movies/1080.mp4");
    ChaoPlayerPorxy::Get()->Start();
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_ChaoPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    ChaoPlayerPorxy::Get()->InitView(win);
}

