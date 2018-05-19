package com.lichao.chaoplayer.httpservice.service;

/**
 * Created by ChaoLi on 2018/5/19 0019 - 9:03
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class ExceptionApi extends RuntimeException {

    private int code;
    private String msg;

    public ExceptionApi(int resultCode, String msg) {
        this.code = resultCode;
        this.msg = msg;
    }

    public int getCode() {
        return code;
    }

    public String getMsg() {
        return msg;
    }
}
