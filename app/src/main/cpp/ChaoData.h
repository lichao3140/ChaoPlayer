//
// Created by Administrator on 2018/5/4 0004.
//

#ifndef CHAOPLAYER_CHAODATA_H
#define CHAOPLAYER_CHAODATA_H


struct ChaoData {
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};// 解码后的数据
    int size = 0;
    bool isAudio = false;
    int width = 0;
    int height = 0;
    void Drop();
};


#endif //CHAOPLAYER_CHAODATA_H
