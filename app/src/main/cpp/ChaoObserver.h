//
// Created by Administrator on 2018/5/4 0004.
//

#ifndef CHAOPLAYER_CHAOOBSERVER_H
#define CHAOPLAYER_CHAOOBSERVER_H

#include "ChaoThread.h"
#include "ChaoData.h"
#include <vector>
#include <mutex>

//观察者 和 主体
class ChaoObserver: public ChaoThread {
public:
    //观察者接收数据函数
    virtual void Update(ChaoData data) {}

    //主体函数 添加观察者(线程安全)
    void AddObs(ChaoObserver *obs);

    //通知所有观察者(线程安全)
    void Notify(ChaoData data);

protected:
    std::vector<ChaoObserver *>obss;
    std::mutex mux;  // C++ 11支持
};


#endif //CHAOPLAYER_CHAOOBSERVER_H
