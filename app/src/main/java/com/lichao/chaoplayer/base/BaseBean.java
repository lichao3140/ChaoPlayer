package com.lichao.chaoplayer.base;

import com.lichao.chaoplayer.util.BeanUtil;

import java.io.Serializable;

/**
 * Created by ChaoLi on 2018/5/18 0018 - 12:31
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class BaseBean implements Serializable {

    public static final long serialVersionUID = -316172390920775219L;

    @Override
    public String toString() {
        return BeanUtil.bean2string(this);
    }

}
