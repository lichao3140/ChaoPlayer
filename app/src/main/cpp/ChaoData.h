//
// Created by Administrator on 2018/5/4 0004.
//

#ifndef CHAOPLAYER_CHAODATA_H
#define CHAOPLAYER_CHAODATA_H


struct ChaoData {
    unsigned char *data = 0;
    int size = 0;
    bool isAudio = false;
    void Drop();
};


#endif //CHAOPLAYER_CHAODATA_H
