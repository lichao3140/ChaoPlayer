//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_FFDECODE_H
#define CHAOPLAYER_FFDECODE_H

#include "ChaoParameter.h"
#include "ChaoDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode:public ChaoDecode {
public:
    //打开解码器
    virtual bool Open(ChaoParameter para);

    //future模型 发送数据到线程解码
    virtual bool SendPacket(ChaoData pkt);

    //从线程中获取解码结果  再次调用会复用上次空间，线程不安全
    virtual ChaoData RecvFrame();

protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
};


#endif //CHAOPLAYER_FFDECODE_H
