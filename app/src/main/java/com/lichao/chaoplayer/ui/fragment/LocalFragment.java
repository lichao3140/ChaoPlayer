package com.lichao.chaoplayer.ui.fragment;

import android.os.Bundle;
import android.support.annotation.Nullable;

import com.lichao.chaoplayer.R;
import com.lichao.chaoplayer.base.BaseFragment;

/**
 * Created by ChaoLi on 2018/5/18 0018 - 12:47
 * Email: lichao3140@gmail.com
 * Version: v1.0
 */
public class LocalFragment extends BaseFragment {

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.fragment_local_layout);
    }
}
