//
// Created by Administrator on 2018/5/4 0004.
//

#ifndef CHAOPLAYER_CHAODEMUX_H
#define CHAOPLAYER_CHAODEMUX_H

#include "ChaoData.h"
#include "ChaoObserver.h"
#include "ChaoParameter.h"

//解封装接口
class ChaoDemux: public ChaoObserver {
public:
    //打开文件，或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url) = 0;

    //virtual void Close();

    //获取视频参数
    virtual ChaoParameter GetVPara() = 0;

    //获取音频参数
    virtual ChaoParameter GetAPara() = 0;

    //读取一帧数据，数据由调用者清理
    virtual ChaoData Read() = 0;

    //总时长（毫秒）
    int totalMs = 0;
protected:
    virtual void Main();
};


#endif //CHAOPLAYER_CHAODEMUX_H
