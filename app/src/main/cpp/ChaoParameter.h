//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOPARAMETER_H
#define CHAOPLAYER_CHAOPARAMETER_H

struct AVCodecParameters;
class ChaoParameter {
public:
    AVCodecParameters *para = 0;
    int channels = 2;
    int sample_rate = 44100;
};


#endif //CHAOPLAYER_CHAOPARAMETER_H
