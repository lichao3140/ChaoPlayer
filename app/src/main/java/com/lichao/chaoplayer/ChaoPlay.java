package com.lichao.chaoplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

public class ChaoPlay extends GLSurfaceView implements Runnable, SurfaceHolder.Callback {

    public ChaoPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void run() {
        //Open("/sdcard/Movies/1080.mp4", getHolder().getSurface());
        //Yuv("/sdcard/Movies/out.yuv", getHolder().getSurface());
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        //new Thread(this).start();
        //初始化opengl egl 显示
        InitView(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public native void Open(String url, Object surface);
    public native void Yuv(String url, Object surface);
    public native void InitView(Object surface);
}
