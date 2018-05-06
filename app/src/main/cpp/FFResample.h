//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_FFRESAMPLE_H
#define CHAOPLAYER_FFRESAMPLE_H


#include "ChaoResample.h"

struct SwrContext;
class FFResample: public ChaoResample {
public:
    virtual bool Open(ChaoParameter in, ChaoParameter out = ChaoParameter());
    virtual void Close();
    virtual ChaoData Resample(ChaoData indata);
protected:
    SwrContext *actx = 0;
    std::mutex mux;
};


#endif //CHAOPLAYER_FFRESAMPLE_H
