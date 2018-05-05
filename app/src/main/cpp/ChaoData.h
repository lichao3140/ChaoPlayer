//
// Created by Administrator on 2018/5/4 0004.
//

#ifndef CHAOPLAYER_CHAODATA_H
#define CHAOPLAYER_CHAODATA_H

enum CHAODataType {
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1
};

struct ChaoData {
    int type = 0;
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};// 解码后的数据
    int size = 0;
    bool isAudio = false;
    int width = 0;
    int height = 0;
    int format = 0; // 视频格式 用来区分使用 软解码或者硬解码
    bool Alloc(int size, const char *data = 0);
    void Drop();
};


#endif //CHAOPLAYER_CHAODATA_H
