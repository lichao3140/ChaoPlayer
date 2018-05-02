#include <jni.h>
#include <string>
#include <android/log.h>
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "lichao", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "lichao", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "lichao", __VA_ARGS__)

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}
#include<iostream>
using namespace std;

// 把分数转换成浮点型
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0 : (double)r.num / (double)r.den;
}
extern "C" JNIEXPORT jstring

JNICALL
Java_com_lichao_chaoplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    //初始化解封装
    av_register_all();
    //初始化网络
    avformat_network_init();
    //打开文件
    AVFormatContext *ic = NULL;
    char path[] = "/sdcard/Movies/1080.mp4";
    int re = avformat_open_input(&ic, path, 0, 0);
    if (re != 0) {
        // 打开失败输出错误原因
        LOGE("avformat_open_input failed!:%s", av_err2str(re));
        hello += " failed!";
        return env->NewStringUTF(hello.c_str());
    }
    LOGI("avformat_open_input %s success!", path);
    // 获取视频流信息 加这句能保证flv格式视频（没格式信息的视频）
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        LOGW("avformat_find_stream_info failed");
    }
    // duration总时长微秒  nb_streams流的数量
    LOGW("duration = %lld nb_streams = %d", ic->duration, ic->nb_streams);

    // 遍历流信息
    int fps = 0; // 帧率
    int videoStream = 0; //视频流
    int audioStream = 1; //音频流

    for (int i = 0; i < ic->nb_streams; i++) {
        AVStream *as = ic->streams[i];
        if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            LOGW("视频数据");
            videoStream = i;
            fps = r2d(as->avg_frame_rate);

            LOGW("fps = %d, width=%d height=%d codecId=%d pixFormat=%d", fps,
                 as->codecpar->width,  //宽度
                 as->codecpar->height,  //高度
                 as->codecpar->codec_id, //编码器
                 as->codecpar->format   //编码器格式
            );
        } else if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            LOGW("音频数据");

            audioStream = i;
            LOGW("sample_rate=%d channels=%d sample_format=%d",
                 as->codecpar->sample_rate,  // 音频采样率
                 as->codecpar->channels,  // 通道数
                 as->codecpar->format  // 格式
            );
        }
    }

    //获取音频流数据
    audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    LOGW("av_find_best_stream audioStream = %d", audioStream);

    //读取帧数据
    AVPacket *pkt = av_packet_alloc();
    for (;;) {
        int re = av_read_frame(ic,pkt);
        if(re != 0) {
            LOGW("读取到结尾处!");
            // 20秒
            int pos = 20 * r2d(ic->streams[videoStream]->time_base);
            // 跳20秒 往后找和找关键帧
            av_seek_frame(ic, videoStream, pos, AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME );
            continue;
        }
        LOGW("stream = %d size =%d pts=%lld flag=%d", pkt->stream_index, pkt->size, pkt->pts, pkt->flags);
        av_packet_unref(pkt);
    }

    // 关闭上下文
    avformat_close_input(&ic);
    hello += " success!";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_lichao_chaoplayer_MainActivity_Open(JNIEnv *env, jobject instance, jstring url_,
                                             jobject handle) {
    const char *url = env->GetStringUTFChars(url_, 0);

    FILE *fp = fopen(url, "rb");
    if (!fp) {
        LOGE("File %s open failed!", url);
    } else {
        LOGI("File %s open success!", url);
        fclose(fp);
    }
    env->ReleaseStringUTFChars(url_, url);
}