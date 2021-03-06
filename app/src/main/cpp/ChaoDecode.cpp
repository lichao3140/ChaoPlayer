//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoDecode.h"

//由主体notify的数据
void ChaoDecode::Update(ChaoData pkt) {
    if(pkt.isAudio != isAudio) {
        return;
    }
    while (!isExit) {
        packsMutex.lock();
        //阻塞
        if(packs.size() < maxList) {
            //生产者
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        ChaoSleep(1);
    }
}

void ChaoDecode::Clear() {
    packsMutex.lock();
    while(!packs.empty()) {
        packs.front().Drop();
        packs.pop_front();
    }
    pts = 0;
    synPts = 0;
    packsMutex.unlock();
}

void ChaoDecode::Main() {
    while(!isExit) {
        //暂停状态
        if(IsPause()) {
            ChaoSleep(2);
            continue;
        }

        packsMutex.lock();
        //判断音视频同步
        if(!isAudio && synPts > 0) {
            if(synPts < pts) {
                packsMutex.unlock();
                ChaoSleep(1);
                continue;
            }
        }

        if(packs.empty()) {
            packsMutex.unlock();
            ChaoSleep(1);
            continue;
        }
        //取出packet 消费者
        ChaoData pack = packs.front();
        packs.pop_front();

        //发送数据到解码线程，一个数据包，可能解码多个结果
        if(this->SendPacket(pack)) {
            while(!isExit) {
                //获取解码数据
                ChaoData frame = RecvFrame();
                if(!frame.data) break;
                //CHAOLOGE("RecvFrame %d", frame.size);
                pts = frame.pts;
                //发送数据给观察者
                this->Notify(frame);
            }
        }
        pack.Drop();//清理  防止内存泄漏
        packsMutex.unlock();
    }
}
