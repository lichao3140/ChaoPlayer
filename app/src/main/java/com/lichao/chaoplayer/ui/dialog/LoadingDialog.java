package com.lichao.chaoplayer.ui.dialog;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.text.TextUtils;
import android.widget.TextView;

import com.lichao.chaoplayer.R;
import com.lichao.chaoplayer.base.BaseDialog;

import butterknife.BindView;

/**
 * Created by ChaoLi on 2018/5/18 0018 - 11:56
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class LoadingDialog extends BaseDialog {

    @BindView(R.id.tv_msg)
    TextView tvMsg;

    public LoadingDialog(@NonNull Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.loading_dialog);
    }

    public void setMsg(String msg) {
        if(tvMsg != null && !TextUtils.isEmpty(msg)) {
            tvMsg.setText(msg);
        }
    }
}
