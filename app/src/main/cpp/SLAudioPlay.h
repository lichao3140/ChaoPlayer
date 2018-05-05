//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_SLAUDIOPLAY_H
#define CHAOPLAYER_SLAUDIOPLAY_H


#include "ChaoAudioPlay.h"

class SLAudioPlay: public ChaoAudioPlay {
public:
    virtual bool StartPlay(ChaoParameter out);
    void PlayCall(void *bufq);

    SLAudioPlay();
    virtual ~SLAudioPlay();
protected:
    unsigned char *buf = 0;
};


#endif //CHAOPLAYER_SLAUDIOPLAY_H
