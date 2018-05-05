package com.lichao.chaoplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class ChaoPlay extends GLSurfaceView implements Runnable, SurfaceHolder.Callback, GLSurfaceView.Renderer {

    public ChaoPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void run() {

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        //初始化opengl egl 显示
        InitView(holder.getSurface());
        //android 8.0 需要设置
        setRenderer(this);
        //只有在绘制数据改变时才绘制view，可以防止GLSurfaceView帧重绘
        //setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }

    public native void InitView(Object surface);

}
