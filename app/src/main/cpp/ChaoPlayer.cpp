//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoPlayer.h"
#include "ChaoDemux.h"
#include "ChaoDecode.h"
#include "ChaoAudioPlay.h"
#include "ChaoVideoView.h"
#include "ChaoResample.h"
#include "ChaoLog.h"

ChaoPlayer *ChaoPlayer::Get(unsigned char index) {
    static ChaoPlayer p[256];
    return &p[index];
}

bool ChaoPlayer::Open(const char *path) {
    //解封装
    if(!demux || !demux->Open(path)) {
        CHAOLOGE("demux->Open %s failed!", path);
        return false;
    }
    //解码 解码可能不需要，如果是解封之后就是原始数据
    if(!vdecode || !vdecode->Open(demux->GetVPara(), isHardDecode)) {
        CHAOLOGE("vdecode->Open %s failed!", path);
    }
    if(!adecode || !adecode->Open(demux->GetAPara())) {
        CHAOLOGE("adecode->Open %s failed!", path);
    }
    //重采样 有可能不需要，解码后或者解封后可能是直接能播放的数据
    if(outPara.sample_rate <= 0)
        outPara = demux->GetAPara();
    if(!resample || !resample->Open(demux->GetAPara(), outPara)) {
        CHAOLOGE("resample->Open %s failed!", path);
    }
    return true;
}

bool ChaoPlayer::Start() {
    if(!demux || !demux->Start()) {
        CHAOLOGE("demux->Start failed!");
        return false;
    }
    if(adecode)
        adecode->Start();
    if(audioPlay)
        audioPlay->StartPlay(outPara);
    if(vdecode)
        vdecode->Start();
    return true;
}

void ChaoPlayer::InitView(void *win) {
    if(videoView)
        videoView->SetRender(win);
}