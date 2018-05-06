//
// Created by Administrator on 2018/5/5 0005.
//

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavcodec/jni.h>
}

#include "FFDecode.h"
#include "ChaoLog.h"

// 硬解码初始化
void FFDecode::InitHard(void *vm) {
    av_jni_set_java_vm(vm,0);
}

//打开解码器
bool FFDecode::Open(ChaoParameter para, bool isHard) {
    Close();
    if(!para.para) return false;
    AVCodecParameters *p = para.para;
    //1 查找解码器
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if(isHard) {
        // 打开硬解码
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if(!cd) {
        CHAOLOGE("avcodec_find_decoder %d failed!  %d", p->codec_id, isHard);
        return false;
    }
    CHAOLOGI("avcodec_find_decoder success %d!", isHard);

    mux.lock();
    //2 创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);

    codec->thread_count = 8;
    //3 打开解码器
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        CHAOLOGE("%s", buf);
        return false;
    }
    // 字幕解码没考虑进来
    if(codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->isAudio = false;
    } else {
        this->isAudio = true;
    }

    mux.unlock();
    CHAOLOGI("avcodec_open2 success!");
    return true;
}

void FFDecode::Clear() {
    ChaoDecode::Clear();
    mux.lock();
    if(codec)
        avcodec_flush_buffers(codec);
    mux.unlock();
}

void FFDecode::Close() {
    ChaoDecode::Clear();
    mux.lock();
    pts = 0;
    if(frame)
        av_frame_free(&frame);
    if(codec) {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mux.unlock();
}

//future模型 发送数据到线程解码
bool FFDecode::SendPacket(ChaoData pkt) {
    if(pkt.size<=0 || !pkt.data)return false;
    mux.lock();
    if(!codec) {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec, (AVPacket*)pkt.data);
    mux.unlock();
    if(re != 0){
        return false;
    }

    return true;
}

//从线程中获取解码结果  再次调用会复用上次空间，线程不安全
ChaoData FFDecode::RecvFrame() {
    mux.lock();
    if (!codec) {
        mux.unlock();
        return ChaoData();
    }
    if (!frame) {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec, frame);
    if (re != 0) {
        mux.unlock();
        return ChaoData();
    }
    ChaoData d;
    d.data = (unsigned char *)frame;
    if(codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        //视频：
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        d.width = frame->width;
        d.height = frame->height;
    } else {
        //音频：样本字节数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample((AVSampleFormat)frame->format) * frame->nb_samples * 2;
    }
    d.format = frame->format;
//    if(!isAudio)
//        CHAOLOGE("data format is %d", frame->format);
    memcpy(d.datas, frame->data, sizeof(d.datas));
    d.pts = frame->pts;
    pts = d.pts;
    mux.unlock();
    return d;
}
