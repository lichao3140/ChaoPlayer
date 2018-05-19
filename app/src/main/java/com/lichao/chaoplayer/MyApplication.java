package com.lichao.chaoplayer;

import android.app.Application;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 9:53
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class MyApplication extends Application {

    private static MyApplication instance;

    public static MyApplication getInstance() {
        return instance;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
    }
}
