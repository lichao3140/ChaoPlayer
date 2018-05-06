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
    //注册硬解码
    static void InitHard(void *vm);
    //打开解码器
    virtual bool Open(ChaoParameter para, bool isHard = false);

    //关闭资源
    virtual void Close();

    //清理
    virtual void Clear();

    //future模型 发送数据到线程解码
    virtual bool SendPacket(ChaoData pkt);

    //从线程中获取解码结果  再次调用会复用上次空间，线程不安全
    virtual ChaoData RecvFrame();

protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
    std::mutex mux;
};


#endif //CHAOPLAYER_FFDECODE_H
