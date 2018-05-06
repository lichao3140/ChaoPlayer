//
// Created by Administrator on 2018/5/4 0004.
//

#include "ChaoData.h"
extern "C"{
    #include <libavformat/avformat.h>
}

bool ChaoData::Alloc(int size,const char *d) {
    Drop();
    type = UCHAR_TYPE;
    if(size <= 0)return false;
    this->data = new unsigned char[size];
    if(!this->data) return false;
    if(d) {
        memcpy(this->data, d, size);
    }
    this->size = size;
    return true;
}

void ChaoData::Drop() {
    if(!data) return;
    if(type == AVPACKET_TYPE)
        av_packet_free((AVPacket **)&data);// 空间释放
    else
        delete data;
    data = 0;
    size = 0;
}