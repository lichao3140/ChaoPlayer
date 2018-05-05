//
// Created by Administrator on 2018/5/4 0004.
//

#include "ChaoData.h"
extern "C"{
    #include <libavformat/avformat.h>
}

void ChaoData::Drop() {
    if(!data) return;
    av_packet_free((AVPacket **)&data);// 空间释放
    data = 0;
    size = 0;
}