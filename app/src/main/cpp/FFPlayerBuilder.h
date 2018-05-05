//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_FFPLAYERBUILDER_H
#define CHAOPLAYER_FFPLAYERBUILDER_H

#include "ChaoPlayerBuilder.h"

class FFPlayerBuilder:public ChaoPlayerBuilder {
public:
    static void InitHard(void *vm);
    static FFPlayerBuilder *Get() {
        static FFPlayerBuilder ff;
        return &ff;
    }
protected:
    FFPlayerBuilder(){};
    virtual ChaoDemux *CreateDemux();
    virtual ChaoDecode *CreateDecode();
    virtual ChaoResample *CreateResample();
    virtual ChaoVideoView *CreateVideoView();
    virtual ChaoAudioPlay *CreateAudioPlay();
    virtual ChaoPlayer *CreatePlayer(unsigned char index = 0);
};


#endif //CHAOPLAYER_FFPLAYERBUILDER_H
