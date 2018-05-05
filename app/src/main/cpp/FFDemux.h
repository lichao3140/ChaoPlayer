//
// Created by Administrator on 2018/5/4 0004.
//

#ifndef CHAOPLAYER_FFDEMUX_H
#define CHAOPLAYER_FFDEMUX_H

#include "ChaoDemux.h"
#include <mutex>
struct AVFormatContext;

class FFDemux: public ChaoDemux {
public:
    //打开文件，或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url);

    virtual void Close();

    //获取视频参数
    virtual ChaoParameter GetVPara();

    //获取音频参数
    virtual ChaoParameter GetAPara();

    //读取一帧数据，数据由调用者清理
    virtual ChaoData Read();

    FFDemux();

private:
    AVFormatContext *ic = 0;
    std::mutex mux;
    int audioStream = 1;
    int videoStream = 0;
};


#endif //CHAOPLAYER_FFDEMUX_H
