package com.lichao.chaoplayer.httpservice.service;


import com.lichao.chaoplayer.httpservice.httpentity.VideoHttpResult;

import io.reactivex.Observable;
import io.reactivex.Observer;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.functions.Function;
import io.reactivex.schedulers.Schedulers;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 8:42
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class VideoBaseApi {

    public <T> void toSubscribe(Observable<T> o, Observer<T> s) {
        o.subscribeOn(Schedulers.io())
                .unsubscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(s);
    }

    public class HttpResultFunc<T> implements Function<VideoHttpResult<T>, T> {

        @Override
        public T apply(VideoHttpResult<T> videoHttpResult) {
            if (videoHttpResult.getErrno() == 0) {
                return videoHttpResult.getData();
            }
            throw new ExceptionApi(videoHttpResult.getErrno(), videoHttpResult.getErrmsg());
        }
    }
}
