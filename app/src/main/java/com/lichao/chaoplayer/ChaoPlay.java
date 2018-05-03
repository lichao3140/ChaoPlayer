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
        Open("/sdcard/Movies/1080.mp4", getHolder().getSurface());
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
       new Thread(this).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public native void Open(String url, Object surface);
}
