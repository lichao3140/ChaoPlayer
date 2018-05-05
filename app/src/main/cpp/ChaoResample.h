//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAORESAMPLE_H
#define CHAOPLAYER_CHAORESAMPLE_H


#include "ChaoObserver.h"
#include "ChaoParameter.h"

class ChaoResample: public ChaoObserver {
public:
    virtual bool Open(ChaoParameter in, ChaoParameter out = ChaoParameter()) = 0;
    virtual ChaoData Resample(ChaoData indata) = 0;
    virtual void Update(ChaoData data);
    int outChannels = 2;
    int outFormat = 1;
};


#endif //CHAOPLAYER_CHAORESAMPLE_H
