package com.lichao.chaoplayer.lcpplayer.listener;

import android.graphics.Bitmap;
import android.view.Surface;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 13:28
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public interface LCOnGlSurfaceViewOncreateListener {

    void onGlSurfaceViewOncreate(Surface surface);

    void onCutVideoImg(Bitmap bitmap);
}
