//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoAudioPlay.h"

void ChaoAudioPlay::Clear() {
    framesMutex.lock();
    while(!frames.empty()) {
        frames.front().Drop();
        frames.pop_front();
    }
    framesMutex.unlock();
}

ChaoData ChaoAudioPlay::GetData() {
    ChaoData d;

    while(!isExit) {
        framesMutex.lock();
        if(!frames.empty()) {
            //有数据返回
            d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            pts = d.pts;
            return d;
        }
        framesMutex.unlock();
        ChaoSleep(1);
    }
    //未获取数据
    return d;
}

void ChaoAudioPlay::Update(ChaoData data) {
    //CHAOLOGE("IAudioPlay::Update %d", data.size);
    //压入缓冲队列
    if(data.size <= 0 || !data.data) return;
    while(!isExit) {
        framesMutex.lock();
        if(frames.size() > maxFrame) {
            framesMutex.unlock();
            ChaoSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }
}