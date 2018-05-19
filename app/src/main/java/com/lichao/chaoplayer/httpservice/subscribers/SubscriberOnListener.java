package com.lichao.chaoplayer.httpservice.subscribers;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 9:17
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public interface SubscriberOnListener<T> {

    void onSucceed(T data);

    void onError(int code, String msg);
}
