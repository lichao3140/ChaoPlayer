//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_SLAUDIOPLAY_H
#define CHAOPLAYER_SLAUDIOPLAY_H


#include "ChaoAudioPlay.h"

class SLAudioPlay: public ChaoAudioPlay {
public:
    virtual bool StartPlay(ChaoParameter out);
    virtual void Close();
    void PlayCall(void *bufq);

    SLAudioPlay();
    virtual ~SLAudioPlay();
protected:
    unsigned char *buf = 0;
    std::mutex mux;
};


#endif //CHAOPLAYER_SLAUDIOPLAY_H
