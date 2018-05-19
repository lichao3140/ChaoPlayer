package com.lichao.chaoplayer.util;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 12:06
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class MyNative {

    /**
     * 打开加载视频
     */
    public native static void Open(String url);

    /**
     * 初始化
     */
    public native static void InitView(Object surface);

    /**
     * 暂停
     */
    public native static void PlayOrPause();

    /**
     * 播放进度条
     */
    public native static double PlayPos();

    /**
     * 进度条
     */
    public native static void Seek(double pos);

}
