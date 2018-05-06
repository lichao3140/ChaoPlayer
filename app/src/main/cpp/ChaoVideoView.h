//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOVIDEOVIEW_H
#define CHAOPLAYER_CHAOVIDEOVIEW_H


#include "ChaoData.h"
#include "ChaoObserver.h"

class ChaoVideoView:public ChaoObserver {
public:
    virtual void SetRender(void *win) = 0;
    virtual void Render(ChaoData data) = 0;
    virtual void Update(ChaoData data);
    virtual void Close() = 0;
};


#endif //CHAOPLAYER_CHAOVIDEOVIEW_H
