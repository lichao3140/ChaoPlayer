//
// Created by Administrator on 2018/5/4 0004.
//

#include "FFDemux.h"
#include "ChaoLog.h"

extern "C" {
    #include <libavformat/avformat.h>
}

//打开文件，或者流媒体 rmtp http rtsp
bool FFDemux::Open(const char *url) {
    CHAOLOGI("Open file %s begin", url);
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        CHAOLOGE("FFDemux open %s failed!", url);
        return false;
    }
    CHAOLOGI("FFDemux open %s success!", url);

    //读取文件信息
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf));
        CHAOLOGE("avformat_find_stream_info %s failed!", url);
        return false;
    }
    this->totalMs = ic->duration / (AV_TIME_BASE / 1000);
    CHAOLOGI("total = %d ms!", totalMs);

    return true;
}

//读取一帧数据，数据由调用者清理
ChaoData FFDemux::Read() {
    ChaoData d;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if(re != 0) {
        av_packet_free(&pkt);//释放空间
        return ChaoData();
    }
    //CHAOLOGI("pack size is %d ptss %lld", pkt->size, pkt->pts);
    d.data = (unsigned char*)pkt;
    d.size = pkt->size;
    if(pkt->stream_index == audioStream) {
        d.isAudio = true;
    } else if(pkt->stream_index == videoStream) {
        d.isAudio = false;
    } else {
        av_packet_free(&pkt);
        return ChaoData();
    }
    return d;
}

FFDemux::FFDemux() {
    //第一次进来  不能保证线程安全
    static bool isFirst = true;
    if(isFirst) {
        isFirst = false;
        //注册所有封装器
        av_register_all();

        //注册所有的解码器
        avcodec_register_all();

        //初始化网络
        avformat_network_init();
        CHAOLOGI("register ffmpeg!");
    }
}