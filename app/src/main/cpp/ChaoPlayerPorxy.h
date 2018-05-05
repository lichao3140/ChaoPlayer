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
    virtual bool Start();
    virtual void InitView(void *win);

protected:
    ChaoPlayerPorxy(){}
    ChaoPlayer *player = 0;
    std::mutex mux;
};


#endif //CHAOPLAYER_CHAOPLAYERPORXY_H
