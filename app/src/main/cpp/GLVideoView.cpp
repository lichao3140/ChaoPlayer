//
// Created by Administrator on 2018/5/5 0005.
//

#include "GLVideoView.h"
#include "ChaoTexture.h"

void GLVideoView::SetRender(void *win) {
    view = win;
}

void GLVideoView::Render(ChaoData data) {
    if(!view) return;
    if(!txt) {
        txt = ChaoTexture::Create();
        txt->Init(view, (ChaoTextureType)data.format);
    }
    txt->Draw(data.datas, data.width, data.height);
}
