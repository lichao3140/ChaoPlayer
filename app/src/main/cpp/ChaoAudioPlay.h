//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOAUDIOPLAY_H
#define CHAOPLAYER_CHAOAUDIOPLAY_H

#include <list>
#include "ChaoObserver.h"
#include "ChaoParameter.h"

class ChaoAudioPlay: public ChaoObserver {
public:
    //缓冲满后阻塞
    virtual void Update(ChaoData data);
    //获取缓冲数据，如没有则阻塞
    virtual ChaoData GetData();
    //开始播放
    virtual bool StartPlay(ChaoParameter out) = 0;
    virtual void Close() = 0;
    virtual void Clear();
    //最大缓冲
    int maxFrame = 100;

    int pts = 0;
protected:
    std::list <ChaoData> frames;
    //  互斥变量
    std::mutex framesMutex;
};


#endif //CHAOPLAYER_CHAOAUDIOPLAY_H
