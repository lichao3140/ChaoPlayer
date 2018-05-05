//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoAudioPlay.h"
#include "ChaoLog.h"

ChaoData ChaoAudioPlay::GetData() {
    ChaoData d;

    while(!isExit) {
        framesMutex.lock();
        if(!frames.empty()) {
            d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            return d;
        }
        framesMutex.unlock();
        ChaoSleep(1);
    }
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