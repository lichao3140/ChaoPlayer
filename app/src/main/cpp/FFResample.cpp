//
// Created by Administrator on 2018/5/5 0005.
//

extern "C" {
    #include <libswresample/swresample.h>
}

#include "ChaoLog.h"
#include <libavcodec/avcodec.h>
#include "FFResample.h"

bool FFResample::Open(ChaoParameter in, ChaoParameter out) {
    Close();
    mux.lock();
    //音频重采样上下文初始化
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(out.channels),
                              AV_SAMPLE_FMT_S16,out.sample_rate,
                              av_get_default_channel_layout(in.para->channels),
                              (AVSampleFormat)in.para->format,in.para->sample_rate,
                              0,0 );
    int re = swr_init(actx);
    if(re != 0) {
        mux.unlock();
        CHAOLOGE("swr_init failed!");
        return false;
    } else {
        CHAOLOGI("swr_init success!");
    }
    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    mux.unlock();
    return true;
}

void FFResample::Close() {
    mux.lock();
    if(actx) {
        swr_free(&actx);
    }
    mux.unlock();
}

// 音频重采样
ChaoData FFResample::Resample(ChaoData indata) {
    if(indata.size<=0 || !indata.data) return ChaoData();
    mux.lock();
    if(!actx) {
        mux.unlock();
        return ChaoData();
    }
    //CHAOLOGE("indata size is %d", indata.size);
    AVFrame *frame = (AVFrame *)indata.data;

    //输出空间的分配
    ChaoData out;
    int outsize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if(outsize <=0)return ChaoData();
    out.Alloc(outsize);

    // 输出的大小
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx, outArr,frame->nb_samples, (const uint8_t **)frame->data, frame->nb_samples);
    if(len <= 0) {
        mux.unlock();
        out.Drop();
        return ChaoData();
    }
    out.pts = indata.pts;
    mux.unlock();
    //CHAOLOGE("swr_convert success = %d", len);
    return out;
}