//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoPlayerPorxy.h"
#include "FFPlayerBuilder.h"

void ChaoPlayerPorxy::Close() {
    mux.lock();
    if(player)
        player->Close();
    mux.unlock();
}

void ChaoPlayerPorxy::Init(void *vm) {
    mux.lock();
    if(vm) {
        FFPlayerBuilder::InitHard(vm);
    }
    if(!player)
        player = FFPlayerBuilder::Get()->BuilderPlayer();
    mux.unlock();
}

bool ChaoPlayerPorxy::Open(const char *path) {
    bool re = false;
    mux.lock();
    if(player)
        re = player->Open(path);
    mux.unlock();
    return re;
}

bool ChaoPlayerPorxy::Start()
{
    bool re = false;
    mux.lock();
    if(player)
        re = player->Start();
    mux.unlock();
    return re;
}

void ChaoPlayerPorxy::InitView(void *win) {
    mux.lock();
    if(player)
        player->InitView(win);
    mux.unlock();
}
