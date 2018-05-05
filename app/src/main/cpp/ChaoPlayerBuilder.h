//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOPLAYERBUILDER_H
#define CHAOPLAYER_CHAOPLAYERBUILDER_H

#include "ChaoPlayer.h"

class ChaoPlayerBuilder {
public:
    virtual ChaoPlayer *BuilderPlayer(unsigned char index=0);

protected:
    virtual ChaoDemux *CreateDemux() = 0;
    virtual ChaoDecode *CreateDecode() = 0;
    virtual ChaoResample *CreateResample() = 0;
    virtual ChaoVideoView *CreateVideoView()  = 0;
    virtual ChaoAudioPlay *CreateAudioPlay() = 0;
    virtual ChaoPlayer *CreatePlayer(unsigned char index = 0) = 0;
};


#endif //CHAOPLAYER_CHAOPLAYERBUILDER_H
