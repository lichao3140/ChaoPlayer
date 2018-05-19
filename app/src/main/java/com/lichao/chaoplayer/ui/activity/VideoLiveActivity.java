package com.lichao.chaoplayer.ui.activity;

import android.annotation.SuppressLint;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.RequiresApi;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;
import com.lichao.chaoplayer.R;
import com.lichao.chaoplayer.base.BaseActivity;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnCompleteListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnErrorListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnInfoListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnLoadListener;
import com.lichao.chaoplayer.lcpplayer.listener.LCOnPreparedListener;
import com.lichao.chaoplayer.lcpplayer.player.LCPlayer;
import com.lichao.chaoplayer.lcpplayer.player.LCTimeBean;
import com.lichao.chaoplayer.ui.widget.ChaoPlay;
import com.lichao.chaoplayer.util.MyLog;
import butterknife.BindView;
import butterknife.ButterKnife;

/**
 * 视频播放页面
 */
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

    private LCPlayer lcPlayer;
    private String pathurl;

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_live);
        ButterKnife.bind(this);

        pathurl = getIntent().getExtras().getString("url");

        lcPlayer = new LCPlayer();
        lcPlayer.Open(pathurl);
        lcPlayer.setlcOnErrorListener(new LCOnErrorListener() {
            @Override
            public void onError(int code, String msg) {
                MyLog.d("code:" + code + ",msg:" + msg);
                Message message = Message.obtain();
                message.obj = msg;
                message.what = 3;
                handler.sendMessage(message);
            }
        });

        lcPlayer.setlcOnPreparedListener(new LCOnPreparedListener() {
            @Override
            public void onPrepared() {
                MyLog.d("starting......");
                lcPlayer.start();
            }
        });

        lcPlayer.setlcOnLoadListener(new LCOnLoadListener() {
            @Override
            public void onLoad(boolean load) {
                MyLog.d("loading ......" + load);
                Message message = Message.obtain();
                message.what = 1;
                message.obj = load;
                handler.sendMessage(message);
            }
        });

        lcPlayer.setlcOnInfoListener(new LCOnInfoListener() {
            @Override
            public void onInfo(LCTimeBean lcTimeBean) {
                Message message = Message.obtain();
                message.what = 2;
                message.obj = lcTimeBean;
                MyLog.d("nowTime is " + lcTimeBean.getCurrt_secds());
                handler.sendMessage(message);
            }
        });

        lcPlayer.setlcOnCompleteListener(new LCOnCompleteListener() {
            @Override
            public void onComplete() {
                MyLog.d("complete......");
                lcPlayer.stop(true);
            }
        });


    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    @Override
    public void onBackPressed() {
        if (lcPlayer != null) {
            lcPlayer.stop(true);
        }
        this.finish();
    }

    @SuppressLint("HandlerLeak")
    Handler handler = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg.what == 1) {
                boolean load = (boolean) msg.obj;
                if (load) {
                    pbLoading.setVisibility(View.VISIBLE);
                } else {
                    if (lyAction.getVisibility() != View.VISIBLE) {
                        lyAction.setVisibility(View.VISIBLE);
                        ivCutimg.setVisibility(View.VISIBLE);
                    }
                    pbLoading.setVisibility(View.GONE);
                }
            } else if (msg.what == 2) {

            }
        }
    };
}
