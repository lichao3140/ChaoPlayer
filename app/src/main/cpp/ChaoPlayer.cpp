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

void ChaoPlayer::Main() {
    while (!isExit) {
        mux.lock();
        if(!audioPlay|| !vdecode) {
            mux.unlock();
            ChaoSleep(2);
            continue;
        }
        //同步
        //获取音频的pts 告诉视频
        int apts = audioPlay->pts;
        //CHAOLOGE("apts = %d", apts);
        vdecode->synPts = apts;

        mux.unlock();
        ChaoSleep(2);
    }
}

void ChaoPlayer::Close() {
    mux.lock();
    //2 先关闭主体线程，再清理观察者
    //同步线程
    ChaoThread::Stop();
    //解封装
    if(demux)
        demux->Stop();
    //解码
    if(vdecode)
        vdecode->Stop();
    if(adecode)
        adecode->Stop();
    if(audioPlay)
        audioPlay->Stop();

    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear();
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();

    //3 清理资源
    if(audioPlay)
        audioPlay->Close();
    if(videoView)
        videoView->Close();
    if(vdecode)
        vdecode->Close();
    if(adecode)
        adecode->Close();
    if(demux)
        demux->Close();
    mux.unlock();
}

double ChaoPlayer::PlayPos() {
    double pos = 0.0;
    mux.lock();

    int total = 0;
    if(demux)
        total = demux->totalMs;
    if(total>0) {
        if(vdecode) {
            pos = (double)vdecode->pts/(double)total;
        }
    }
    mux.unlock();
    return pos;
}

void ChaoPlayer::SetPause(bool isP) {
    mux.lock();
    ChaoThread::SetPause(isP);
    if(demux)
        demux->SetPause(isP);
    if(vdecode)
        vdecode->SetPause(isP);
    if(adecode)
        adecode->SetPause(isP);
    if(audioPlay)
        audioPlay->SetPause(isP);
    mux.unlock();
}

bool ChaoPlayer::Seek(double pos) {
    bool re = false;
    if(!demux) return false;

    //暂停所有线程
    SetPause(true);
    mux.lock();
    //清理缓冲
    //2 清理缓冲队列
    if(vdecode)
        vdecode->Clear(); //清理缓冲队列，清理ffmpeg的缓冲
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();

    re = demux->Seek(pos); //seek跳转到关键帧
    if(!vdecode) {
        mux.unlock();
        SetPause(false);
        return re;
    }
    //解码到实际需要显示的帧
    int seekPts = pos*demux->totalMs;
    while(!isExit) {
        ChaoData pkt = demux->Read();
        if(pkt.size<=0)break;
        if(pkt.isAudio) {
            if(pkt.pts < seekPts) {
                pkt.Drop();
                continue;
            }
            //写入缓冲队列
            demux->Notify(pkt);
            continue;
        }

        //解码需要显示的帧之前的数据
        vdecode->SendPacket(pkt);
        pkt.Drop();
        ChaoData data = vdecode->RecvFrame();
        if(data.size <=0) {
            continue;
        }
        if(data.pts >= seekPts) {
            //vdecode->Notify(data);
            break;
        }
    }
    mux.unlock();

    SetPause(false);
    return re;
}

bool ChaoPlayer::Open(const char *path) {
    Close();
    mux.lock();
    //解封装
    if(!demux || !demux->Open(path)) {
        mux.unlock();
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
    outPara = demux->GetAPara();
    if(!resample || !resample->Open(demux->GetAPara(), outPara)) {
        CHAOLOGE("resample->Open %s failed!", path);
    }
    mux.unlock();
    return true;
}

bool ChaoPlayer::Start() {
    mux.lock();
    if(vdecode)
        vdecode->Start();
    if(!demux || !demux->Start()) {
        mux.unlock();
        CHAOLOGE("demux->Start failed!");
        return false;
    }
    if(adecode)
        adecode->Start();
    if(audioPlay)
        audioPlay->StartPlay(outPara);
    ChaoThread::Start();
    mux.unlock();
    return true;
}

void ChaoPlayer::InitView(void *win) {
    if(videoView) {
        videoView->Close();
        videoView->SetRender(win);
    }
}
