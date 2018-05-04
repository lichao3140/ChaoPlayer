//
// Created by Administrator on 2018/5/4 0004.
//

#include "ChaoObserver.h"

//主体函数 添加观察者
void ChaoObserver::AddObs(ChaoObserver *obs) {
    if(!obs) return;
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}

//通知所有观察者
void ChaoObserver::Notify(ChaoData data) {
    mux.lock();
    for(int i =0; i < obss.size(); i++) {
        obss[i]->Update(data);
    }
    mux.unlock();
}