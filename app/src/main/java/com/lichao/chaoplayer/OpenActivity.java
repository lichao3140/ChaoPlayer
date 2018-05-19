package com.lichao.chaoplayer;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class OpenActivity extends AppCompatActivity {

    @BindView(R.id.fileurl)
    EditText fileurl;
    @BindView(R.id.playvideo)
    Button playvideo;
    @BindView(R.id.rtmpurl)
    EditText rtmpurl;
    @BindView(R.id.playrtmp)
    Button playrtmp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open);
        ButterKnife.bind(this);

    }

    @OnClick({R.id.playvideo, R.id.playrtmp})
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.playvideo:
                //MyNative.Open(fileurl.getText().toString());
                finish();
                break;
            case R.id.playrtmp:
                //MyNative.Open(rtmpurl.getText().toString());
                finish();
                break;
            default:
                break;
        }
    }


}
