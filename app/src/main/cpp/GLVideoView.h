//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_GLVIDEOVIEW_H
#define CHAOPLAYER_GLVIDEOVIEW_H


#include "ChaoVideoView.h"

class ChaoTexture;

class GLVideoView: public ChaoVideoView {
public:
    virtual void SetRender(void *win);
    virtual void Render(ChaoData data);

protected:
    void *view = 0;
    ChaoTexture *txt = 0;
};


#endif //CHAOPLAYER_GLVIDEOVIEW_H
