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

//获取当前的播放进度 0.0 ~ 1.0
double ChaoPlayerPorxy::PlayPos() {
    double pos = 0.0;
    mux.lock();
    if(player) {
        pos = player->PlayPos();
    }
    mux.unlock();
    return pos;
}

bool ChaoPlayerPorxy::IsPause() {
    bool re = false;
    mux.lock();
    if(player)
        re = player->IsPause();
    mux.unlock();
    return re;
}

void ChaoPlayerPorxy::SetPause(bool isP) {
    mux.lock();
    if(player)
        player->SetPause(isP);
    mux.unlock();
}

bool ChaoPlayerPorxy::Seek(double pos) {
    bool re = false;
    mux.lock();
    if(player) {
        re = player->Seek(pos);
    }
    mux.unlock();
    return re;
}

bool ChaoPlayerPorxy::Open(const char *path) {
    bool re = false;
    mux.lock();
    if(player) {
        player->isHardDecode = isHardDecode;
        re = player->Open(path);
    }
    mux.unlock();
    return re;
}

bool ChaoPlayerPorxy::Start() {
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
