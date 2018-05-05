//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOPLAYER_H
#define CHAOPLAYER_CHAOPLAYER_H


#include <mutex>
#include "ChaoThread.h"
#include "ChaoParameter.h"

class ChaoDemux;
class ChaoAudioPlay;
class ChaoVideoView;
class ChaoResample;
class ChaoDecode;

class ChaoPlayer: public ChaoThread {
public:
    // 对象的维护
    static ChaoPlayer *Get(unsigned char index = 0);

    virtual bool Open(const char *path);

    virtual bool Start();

    virtual void InitView(void *win);

    //是否视频硬解码
    bool isHardDecode = true;

    //音频输出参数配置
    ChaoParameter outPara;

    ChaoDemux *demux = 0;
    ChaoDecode *vdecode = 0;
    ChaoDecode *adecode = 0;
    ChaoResample *resample = 0;
    ChaoVideoView *videoView = 0;
    ChaoAudioPlay *audioPlay = 0;

protected:
    //用作音视频同步
    void Main();
    std::mutex mux;
    ChaoPlayer(){};
};


#endif //CHAOPLAYER_CHAOPLAYER_H
