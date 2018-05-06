//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOPLAYERPORXY_H
#define CHAOPLAYER_CHAOPLAYERPORXY_H

#include "ChaoPlayer.h"
#include <mutex>

class ChaoPlayerPorxy: public ChaoPlayer {
public:
    static ChaoPlayerPorxy*Get() {
        static ChaoPlayerPorxy px;
        return &px;
    }
    void Init(void *vm = 0);

    virtual bool Open(const char *path);
    virtual bool Seek(double pos);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void *win);
    virtual void SetPause(bool isP);
    virtual bool IsPause();

    //获取当前的播放进度 0.0 ~ 1.0
    virtual double PlayPos();
protected:
    ChaoPlayerPorxy(){}
    ChaoPlayer *player = 0;
    std::mutex mux;
};


#endif //CHAOPLAYER_CHAOPLAYERPORXY_H
