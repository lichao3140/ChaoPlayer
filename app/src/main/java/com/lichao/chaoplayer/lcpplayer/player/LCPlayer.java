package com.lichao.chaoplayer.lcpplayer.player;

import android.graphics.Bitmap;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.text.TextUtils;
import android.view.Surface;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnCompleteListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnCutVideoImgListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnErrorListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnGlSurfaceViewOncreateListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnInfoListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnLoadListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnPreparedListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnStopListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCStatus;
import com.lichao.chaoplayer.lcpplayer.opengles.LCGlSurfaceView;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 13:55
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
public class LCPlayer {

    static {
        System.loadLibrary("native-lib");
    }

    /**
     * 打开加载视频
     */
    public native void Open(String url);

    /**
     * 暂停
     */
    private native void Pause();

    /**
     * 初始化
     */
    public native void InitView(Object surface);

    /**
     * 暂停
     */
    public native void PlayOrPause();

    /**
     * 播放进度条
     */
    public native double PlayPos();

    /**
     * 进度条
     */
    public native void Seek(double pos);


    /**
     * 播放文件路径
     */
    private String dataSource;
    /**
     * 硬解码mime
     */
    private MediaFormat mediaFormat;
    /**
     * 视频硬解码器
     */
    private MediaCodec mediaCodec;
    /**
     * 渲染surface
     */
    private Surface surface;
    /**
     * opengl surfaceview
     */
    private LCGlSurfaceView lcGlSurfaceView;
    /**
     * 视频解码器info
     */
    private MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();

    /**
     * 准备好时的回调
     */
    private LCOnPreparedListener lcOnPreparedListener;
    /**
     * 错误时的回调
     */
    private LCOnErrorListener lcOnErrorListener;
    /**
     * 加载回调
     */
    private LCOnLoadListener lcOnLoadListener;
    /**
     * 更新时间回调
     */
    private LCOnInfoListener lcOnInfoListener;
    /**
     * 播放完成回调
     */
    private LCOnCompleteListener lcOnCompleteListener;
    /**
     * 视频截图回调
     */
    private LCOnCutVideoImgListener lcOnCutVideoImgListener;
    /**
     * 停止完成回调
     */
    private LCOnStopListener lcOnStopListener;
    /**
     * 是否已经准备好
     */
    private boolean parpared = false;
    /**
     * 时长实体类
     */
    private LCTimeBean lcTimeBean;
    /**
     * 上一次播放时间
     */
    private int lastCurrTime = 0;

    /**
     * 是否只有音频（只播放音频流）
     */
    private boolean isOnlyMusic = false;

    private boolean isOnlySoft = false;

    public LCPlayer()
    {
        lcTimeBean = new LCTimeBean();
    }

    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }

    public void setOnlyMusic(boolean onlyMusic) {
        isOnlyMusic = onlyMusic;
    }

    private void setSurface(Surface surface) {
        this.surface = surface;
    }

    public void setlcGlSurfaceView(LCGlSurfaceView lcGlSurfaceView) {
        this.lcGlSurfaceView = lcGlSurfaceView;
        lcGlSurfaceView.setOnGlSurfaceViewOncreateListener(new LCOnGlSurfaceViewOncreateListener() {
            @Override
            public void onGlSurfaceViewOncreate(Surface s) {
                if(surface == null)
                {
                    setSurface(s);
                }
                if(parpared && !TextUtils.isDigitsOnly(dataSource))
                {
                    Open(dataSource);
                }
            }

            @Override
            public void onCutVideoImg(Bitmap bitmap) {
                if(lcOnCutVideoImgListener != null)
                {
                    lcOnCutVideoImgListener.onCutVideoImg(bitmap);
                }
            }
        });
    }

    public void setlcOnPreparedListener(LCOnPreparedListener lcOnPreparedListener) {
        this.lcOnPreparedListener = lcOnPreparedListener;
    }

    public void setlcOnErrorListener(LCOnErrorListener lcOnErrorListener) {
        this.lcOnErrorListener = lcOnErrorListener;
    }

    public void setlcOnLoadListener(LCOnLoadListener lcOnLoadListener) {
        this.lcOnLoadListener = lcOnLoadListener;
    }

    public void setlcOnInfoListener(LCOnInfoListener lcOnInfoListener) {
        this.lcOnInfoListener = lcOnInfoListener;
    }

    public void setlcOnCompleteListener(LCOnCompleteListener lcOnCompleteListener) {
        this.lcOnCompleteListener = lcOnCompleteListener;
    }

    private void onError(int code, String msg) {
        if(lcOnErrorListener != null) {
            lcOnErrorListener.onError(code, msg);
        }
        stop(true);
    }

    private void onLoad(boolean load) {
        if(lcOnLoadListener != null) {
            lcOnLoadListener.onLoad(load);
        }
    }

    public void start() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                if (TextUtils.isEmpty(dataSource)) {
                    onError(LCStatus.WL_STATUS_DATASOURCE_NULL, "datasource is null");
                    return;
                }

                if (!isOnlyMusic) {
                    if(surface == null) {
                        onError(LCStatus.WL_STATUS_SURFACE_NULL, "surface is null");
                        return;
                    }
                }

                if (lcTimeBean == null) {
                    lcTimeBean = new LCTimeBean();
                }
            }
        }).start();
    }

    public void stop(final boolean exit) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Pause();
                if (mediaCodec != null) {
                    try {
                        mediaCodec.flush();
                        mediaCodec.stop();
                        mediaCodec.release();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    mediaCodec = null;
                    mediaFormat = null;
                }
                if (lcGlSurfaceView != null) {
                    lcGlSurfaceView.setCodecType(-1);
                    lcGlSurfaceView.requestRender();
                }
            }
        }).start();
    }

    public void setVideoInfo(int currt_secd, int total_secd) {
        if(lcOnInfoListener != null && lcTimeBean != null) {
            if(currt_secd < lastCurrTime) {
                currt_secd = lastCurrTime;
            }
            lcTimeBean.setCurrt_secds(currt_secd);
            lcTimeBean.setTotal_secds(total_secd);
            lcOnInfoListener.onInfo(lcTimeBean);
            lastCurrTime = currt_secd;
        }
    }

    public void videoComplete() {
        if(lcOnCompleteListener != null) {
            //setVideoInfo(lcGetDuration(), lcGetDuration());
            lcTimeBean = null;
            lcOnCompleteListener.onComplete();
        }
    }
}
