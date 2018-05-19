package com.lichao.chaoplayer.httpservice.subscribers;

import android.content.Context;

import com.lichao.chaoplayer.httpservice.service.ExceptionApi;

import java.net.ConnectException;
import java.net.SocketTimeoutException;

import io.reactivex.Observable;
import io.reactivex.Observer;
import io.reactivex.disposables.Disposable;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 9:18
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class HttpSubscriber<T> implements Observer<T> {

    private SubscriberOnListener subscriberOnListener;
    private Context context;
    private Disposable disposable;

    public HttpSubscriber(SubscriberOnListener subscriberOnListener, Context context) {
        this.subscriberOnListener = subscriberOnListener;
        this.context = context;
    }

    @Override
    public void onSubscribe(Disposable d) {
        disposable = d;
    }

    @Override
    public void onNext(T t) {
        if (subscriberOnListener != null && context != null) {
            subscriberOnListener.onSucceed(t);
        } else {
            if(disposable != null && !disposable.isDisposed())
                disposable.dispose();
        }
    }

    @Override
    public void onError(Throwable e) {
        if(subscriberOnListener != null && context != null) {
            if (e instanceof SocketTimeoutException) {
                subscriberOnListener.onError(-1001, "网络超时，请检查您的网络状态");
            } else if (e instanceof ConnectException) {
                subscriberOnListener.onError(-1002, "网络链接中断，请检查您的网络状态");
            } else if (e instanceof ExceptionApi){
                subscriberOnListener.onError(((ExceptionApi)e).getCode(), ((ExceptionApi)e).getMsg());
            } else {
                subscriberOnListener.onError(-1003, "未知错误:" + e.getMessage());
            }
        } else {
            if(disposable != null && !disposable.isDisposed())
                disposable.dispose();
        }
    }

    @Override
    public void onComplete() {
        if(subscriberOnListener != null && context != null) {

        } else {
            if(disposable != null && !disposable.isDisposed())
                disposable.dispose();
        }
    }
}
