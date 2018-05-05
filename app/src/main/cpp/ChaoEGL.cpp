//
// Created by Administrator on 2018/5/5 0005.
//

#include <android/native_window.h>
#include <EGL/egl.h>
#include "ChaoEGL.h"
#include "ChaoLog.h"

class CChaoEGL:public ChaoEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;

    // 初始化窗口
    virtual bool Init(void *win) {
        ANativeWindow *nwin = (ANativeWindow *)win;

        //初始化EGL
        //1 获取EGLDisplay对象 显示设备
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(display == EGL_NO_DISPLAY) {
            CHAOLOGE("eglGetDisplay failed!");
            return false;
        }
        CHAOLOGE("eglGetDisplay success!");

        //2 初始化Display
        if(EGL_TRUE != eglInitialize(display, 0, 0)) {
            CHAOLOGE("eglInitialize failed!");
            return false;
        }
        CHAOLOGE("eglInitialize success!");

        //3 获取配置并创建surface
        EGLint configSpec [] = {
                EGL_RED_SIZE,8,
                EGL_GREEN_SIZE,8,
                EGL_BLUE_SIZE,8,
                EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfigs = 0;
        if(EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &numConfigs)) {
            CHAOLOGE("eglChooseConfig failed!");
            return false;
        }
        CHAOLOGE("eglChooseConfig success!");
        surface = eglCreateWindowSurface(display, config, nwin, NULL);

        //4 创建并打开EGL上下文
        const EGLint ctxAttr[] = { EGL_CONTEXT_CLIENT_VERSION ,2, EGL_NONE};
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if(context == EGL_NO_CONTEXT) {
            CHAOLOGE("eglCreateContext failed!");
            return false;
        }
        CHAOLOGE("eglCreateContext success!");

        if(EGL_TRUE != eglMakeCurrent(display, surface, surface, context)) {
            CHAOLOGE("eglMakeCurrent failed!");
            return false;
        }
        CHAOLOGE("eglMakeCurrent success!");
        return true;
    }

    virtual void Draw() {
        if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            return;
        }
        eglSwapBuffers(display, surface);
    }
};

ChaoEGL *ChaoEGL::Get() {
    static CChaoEGL egl;
    return &egl;
}