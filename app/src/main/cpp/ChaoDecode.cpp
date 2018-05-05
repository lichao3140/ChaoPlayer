//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoDecode.h"
#include "ChaoLog.h"

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

void ChaoDecode::Main() {
    while(!isExit) {
        packsMutex.lock();
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

                //发送数据给观察者
                this->Notify(frame);
            }
        }
        pack.Drop();//清理  防止内存泄漏
        packsMutex.unlock();
    }
}
