//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAODECODE_H
#define CHAOPLAYER_CHAODECODE_H

#include "ChaoParameter.h"
#include "ChaoObserver.h"
#include <list>

//解码接口，支持硬解码
class ChaoDecode:public ChaoObserver {
public:
    //打开解码器  默认不打开硬解码
    virtual bool Open(ChaoParameter para, bool isHard = false) = 0;

    virtual void Close() = 0;

    virtual void Clear();

    //future模型 发送数据到线程解码
    virtual bool SendPacket(ChaoData pkt) = 0;

    //从线程中获取解码结果  再次调用会复用上次空间，线程不安全
    virtual ChaoData RecvFrame() = 0;

    //由主体notify的数据 阻塞
    virtual void Update(ChaoData pkt);

    bool isAudio = false;

    //最大的队列缓冲
    int maxList = 100;

    //同步时间，再次打开文件要清理
    int synPts = 0;
    int pts = 0;

protected:
    virtual void Main();
    //读取缓冲
    std::list<ChaoData> packs;
    //专门读取队列的互斥变量
    std::mutex packsMutex;
};


#endif //CHAOPLAYER_CHAODECODE_H
