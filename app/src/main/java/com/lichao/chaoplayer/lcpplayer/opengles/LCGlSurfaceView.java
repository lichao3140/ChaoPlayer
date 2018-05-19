package com.lichao.chaoplayer.lcpplayer.opengles;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnGlSurfaceViewOncreateListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnRenderRefreshListener;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 13:48
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class LCGlSurfaceView extends GLSurfaceView {

    private LCGlRender lcGlRender;
    private LCOnGlSurfaceViewOncreateListener onGlSurfaceViewOncreateListener;

    public LCGlSurfaceView(Context context) {
        this(context, null);
    }

    public LCGlSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        lcGlRender = new LCGlRender(context);
        //设置egl版本为2.0
        setEGLContextClientVersion(2);
        //设置render
        setRenderer(lcGlRender);
        //设置为手动刷新模式
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        lcGlRender.setlcOnRenderRefreshListener(new LCOnRenderRefreshListener() {
            @Override
            public void onRefresh() {
                requestRender();
            }
        });
    }

    public void setOnGlSurfaceViewOncreateListener(LCOnGlSurfaceViewOncreateListener onGlSurfaceViewOncreateListener) {
        if(lcGlRender != null)
        {
            lcGlRender.setlcOnGlSurfaceViewOncreateListener(onGlSurfaceViewOncreateListener);
        }
    }

    public void setCodecType(int type)
    {
        if(lcGlRender != null)
        {
            lcGlRender.setCodecType(type);
        }
    }


    public void setFrameData(int w, int h, byte[] y, byte[] u, byte[] v)
    {
        if(lcGlRender != null)
        {
            lcGlRender.setFrameData(w, h, y, u, v);
            requestRender();
        }
    }

    public void cutVideoImg()
    {
        if(lcGlRender != null)
        {
            lcGlRender.cutVideoImg();
            requestRender();
        }
    }
}
