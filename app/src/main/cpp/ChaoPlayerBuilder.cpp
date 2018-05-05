//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoPlayerBuilder.h"
#include "ChaoVideoView.h"
#include "ChaoResample.h"
#include "ChaoDecode.h"
#include "ChaoAudioPlay.h"
#include "ChaoDemux.h"

ChaoPlayer *ChaoPlayerBuilder::BuilderPlayer(unsigned char index) {
    ChaoPlayer *play = CreatePlayer(index);

    //解封装
    ChaoDemux *de = CreateDemux();

    //视频解码
    ChaoDecode *vdecode = CreateDecode();

    //音频解码
    ChaoDecode *adecode = CreateDecode();

    //解码器观察解封装
    de->AddObs(vdecode);
    de->AddObs(adecode);

    //显示观察视频解码器
    ChaoVideoView *view = CreateVideoView();
    vdecode->AddObs(view);

    //重采样观察音频解码器
    ChaoResample *resample = CreateResample();
    adecode->AddObs(resample);

    //音频播放观察重采样
    ChaoAudioPlay *audioPlay = CreateAudioPlay();
    resample->AddObs(audioPlay);

    play->demux = de;
    play->adecode = adecode;
    play->vdecode = vdecode;
    play->videoView = view;
    play->resample = resample;
    play->audioPlay = audioPlay;
    return play;
}
