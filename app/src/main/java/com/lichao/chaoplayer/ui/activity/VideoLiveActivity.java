package com.lichao.chaoplayer.ui.activity;

import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;

import com.lichao.chaoplayer.R;
import com.lichao.chaoplayer.base.BaseActivity;
import com.lichao.chaoplayer.ui.widget.ChaoPlay;
import com.lichao.chaoplayer.util.MyNative;
import com.lichao.chaoplayer.util.TimeUtil;

import butterknife.BindView;
import butterknife.ButterKnife;

public class VideoLiveActivity extends BaseActivity {

    @BindView(R.id.surfaceview)
    ChaoPlay surfaceview;
    @BindView(R.id.pb_loading)
    ProgressBar pbLoading;
    @BindView(R.id.tv_time)
    TextView tvTime;
    @BindView(R.id.seekbar)
    SeekBar seekbar;
    @BindView(R.id.iv_pause)
    ImageView ivPause;
    @BindView(R.id.ly_action)
    LinearLayout lyAction;
    @BindView(R.id.iv_cutimg)
    ImageView ivCutimg;
    @BindView(R.id.iv_show_img)
    ImageView ivShowImg;

    private String pathurl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_live);
        ButterKnife.bind(this);

        pathurl = getIntent().getExtras().getString("url");

        MyNative.Open(pathurl);
        pbLoading.setVisibility(View.GONE);
        //tvTime.setText(TimeUtil.secdsToDateFormat());
    }

    @Override
    public void onBackPressed() {
        //MyNative.PlayOrPause();
        this.finish();
    }


}
