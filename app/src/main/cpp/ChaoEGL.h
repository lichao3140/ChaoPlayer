//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOEGL_H
#define CHAOPLAYER_CHAOEGL_H


class ChaoEGL {
public:
    // 初始化窗口
    virtual bool Init(void *win) = 0;
    virtual void Close() = 0;
    virtual void Draw() = 0;
    static ChaoEGL *Get();

protected:
    ChaoEGL(){}
};


#endif //CHAOPLAYER_CHAOEGL_H
