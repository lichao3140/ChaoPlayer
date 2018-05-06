//
// Created by Administrator on 2018/5/5 0005.
//

#include <android/native_window.h>
#include <EGL/egl.h>
#include <mutex>
#include "ChaoEGL.h"
#include "ChaoLog.h"

class CChaoEGL:public ChaoEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mux;

    // 初始化窗口
    virtual bool Init(void *win) {
        ANativeWindow *nwin = (ANativeWindow *)win;

        //初始化EGL
        mux.lock();
        //1 获取EGLDisplay对象 显示设备
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(display == EGL_NO_DISPLAY) {
            mux.unlock();
            CHAOLOGE("eglGetDisplay failed!");
            return false;
        }
        CHAOLOGE("eglGetDisplay success!");

        //2 初始化Display
        if(EGL_TRUE != eglInitialize(display, 0, 0)) {
            mux.unlock();
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
            mux.unlock();
            CHAOLOGE("eglChooseConfig failed!");
            return false;
        }
        CHAOLOGE("eglChooseConfig success!");
        surface = eglCreateWindowSurface(display, config, nwin, NULL);

        //4 创建并打开EGL上下文
        const EGLint ctxAttr[] = { EGL_CONTEXT_CLIENT_VERSION ,2, EGL_NONE};
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if(context == EGL_NO_CONTEXT) {
            mux.unlock();
            CHAOLOGE("eglCreateContext failed!");
            return false;
        }
        CHAOLOGE("eglCreateContext success!");

        if(EGL_TRUE != eglMakeCurrent(display, surface, surface, context)) {
            mux.unlock();
            CHAOLOGE("eglMakeCurrent failed!");
            return false;
        }
        CHAOLOGE("eglMakeCurrent success!");
        mux.unlock();
        return true;
    }

    virtual void Close() {
        mux.lock();
        if(display == EGL_NO_DISPLAY) {
            mux.unlock();
            return;
        }
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if(surface != EGL_NO_SURFACE)
            eglDestroySurface(display,surface);
        if(context != EGL_NO_CONTEXT)
            eglDestroyContext(display,context);

        eglTerminate(display);

        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        mux.unlock();
    }


    virtual void Draw() {
        mux.lock();
        if(display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            mux.unlock();
            return;
        }
        eglSwapBuffers(display, surface);
        mux.unlock();
    }
};

ChaoEGL *ChaoEGL::Get() {
    static CChaoEGL egl;
    return &egl;
}