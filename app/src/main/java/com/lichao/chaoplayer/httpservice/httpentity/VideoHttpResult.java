package com.lichao.chaoplayer.httpservice.httpentity;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 8:52
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class VideoHttpResult<T> {

    private int errno;
    private String errmsg;
    private T data;

    public int getErrno() {
        return errno;
    }

    public void setErrno(int errno) {
        this.errno = errno;
    }

    public String getErrmsg() {
        return errmsg;
    }

    public void setErrmsg(String errmsg) {
        this.errmsg = errmsg;
    }

    public T getData() {
        return data;
    }

    public void setData(T data) {
        this.data = data;
    }
}
