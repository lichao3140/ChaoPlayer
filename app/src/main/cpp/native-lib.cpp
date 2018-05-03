#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "lichao", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "lichao", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "lichao", __VA_ARGS__)

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavcodec/jni.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
}
#include<iostream>
using namespace std;

// 把分数转换成浮点型
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

//当前时间戳 clock
long long GetNowMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int sec = tv.tv_sec % 360000;
    long long t = sec * 1000 + tv.tv_usec / 1000;
    return t;
}

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    // 调用java 实现硬解码
    av_jni_set_java_vm(vm, 0);
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_lichao_chaoplayer_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";

    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_ChaoPlay_Open(JNIEnv *env, jobject instance, jstring url_, jobject surface) {
    const char *path = env->GetStringUTFChars(url_, 0);

    //初始化解封装
    av_register_all();
    //初始化网络
    avformat_network_init();
    avcodec_register_all();

    //打开文件
    AVFormatContext *ic = NULL;
    int re = avformat_open_input(&ic, path, 0, 0);
    if (re != 0) {
        // 打开失败输出错误原因
        LOGE("avformat_open_input failed!:%s", av_err2str(re));
        return ;
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

    /*********************************打开视频解码器****************************************/
    //解码器--软解码
    AVCodec *codec = avcodec_find_decoder(ic->streams[videoStream]->codecpar->codec_id);
    //硬解码
    codec = avcodec_find_decoder_by_name("h264_mediacodec");
    if (!codec) {
        LOGW("AVCodec find decoder failed");
        return;
    }
    // 视频解码器初始化
    AVCodecContext *vc = avcodec_alloc_context3(codec);
    // 复制视频
    avcodec_parameters_to_context(vc, ic->streams[videoStream]->codecpar);
    // 线程数量
    vc->thread_count = 8;
    // 打开解码器
    re = avcodec_open2(vc, 0, 0);
    if (re != 0) {
        LOGW("avcodec_open2 video failed");
        return;
    }

    /*********************************打开音频解码器****************************************/
    //软解码器
    AVCodec *acodec = avcodec_find_decoder(ic->streams[audioStream]->codecpar->codec_id);
    if (!acodec) {
        LOGW("AVCodec find decoder failed");
        return;
    }
    // 视频解码器初始化
    AVCodecContext *ac = avcodec_alloc_context3(acodec);
    // 复制视频
    avcodec_parameters_to_context(ac, ic->streams[audioStream]->codecpar);
    // 线程数量
    ac->thread_count = 8;
    // 打开解码器
    re = avcodec_open2(ac, 0, 0);
    if (re != 0) {
        LOGW("avcodec_open2 audio failed");
        return;
    }

    //读取帧数据
    AVPacket *pkt = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    long long start = GetNowMs();
    int frameCount = 0;

    //初始化像素格式转换的上下文
    SwsContext *vctx = NULL;
    int outWidth = 1280;
    int outHeight = 720;
    char *rgb = new char[1920*1080*4];
    char *pcm = new char[48000*4*2];

    //音频重采样上下文初始化
    SwrContext *actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(2), // 音频输出通道数
                              AV_SAMPLE_FMT_S16,  //  输出格式
                              ac->sample_rate,  //  输出样本率
                              av_get_default_channel_layout(ac->channels),  //输入格式
                              ac->sample_fmt,//  输入格式
                              ac->sample_rate,  //  输入样本采样率
                              0,0 );
    re = swr_init(actx);
    if(re != 0) {
        LOGW("swr_init failed!");
    } else {
        LOGW("swr_init success!");
    }

    //显示窗口初始化
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    ANativeWindow_setBuffersGeometry(nwin,outWidth,outHeight, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer wbuf;

    for (;;) {
        //超过三秒
        if (GetNowMs() - start >= 3000) {
            LOGW("now decode fps is %d", frameCount / 3);
            start = GetNowMs();
            frameCount = 0;
        }
        int re = av_read_frame(ic, pkt);
        if(re != 0) {
            LOGW("读取到结尾处!");
            // 20秒
            int pos = 20 * r2d(ic->streams[videoStream]->time_base);
            // 跳20秒 往后找和找关键帧
            av_seek_frame(ic, videoStream, pos, AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME );
            continue;
        }
        AVCodecContext *cc = vc;
        if (pkt->stream_index == audioStream)
            cc = ac;
        //LOGW("stream = %d size =%d pts=%lld flag=%d", pkt->stream_index, pkt->size, pkt->pts, pkt->flags);
        // 发送到线程中解码
        re = avcodec_send_packet(cc, pkt);
        // 清理
        int p = pkt->pts;
        av_packet_unref(pkt);

        if (re != 0) {
            // 失败
            LOGW("avcodec_send_packet failed!");
            continue;
        }

        for(;;) {
            re = avcodec_receive_frame(cc, frame);
            if (re != 0) {
                break;
            }
            //LOGW("avcodec_receive_frame %lld", frame->pts);
            // 如果是视频帧
            if(cc == vc) {
                frameCount++;
                // 像素格式初始化
                vctx = sws_getCachedContext(vctx,
                                            frame->width,
                                            frame->height,
                                            (AVPixelFormat)frame->format,
                                            outWidth,
                                            outHeight,
                                            AV_PIX_FMT_RGBA,
                                            SWS_FAST_BILINEAR,
                                            0,0,0);
                if(!vctx) {
                    LOGW("sws_getCachedContext failed!");
                } else {
                    // 显示大小转换
                    uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
                    data[0] =(uint8_t *)rgb;
                    int lines[AV_NUM_DATA_POINTERS] = {0};//一行宽度的大小
                    lines[0] = outWidth * 4;
                    int h = sws_scale(vctx,
                                      (const uint8_t **)frame->data,
                                      frame->linesize,0,
                                      frame->height,
                                      data,lines);
                    LOGW("sws_scale = %d", h);
                    if(h > 0) {
                        ANativeWindow_lock(nwin,&wbuf, 0);
                        uint8_t *dst = (uint8_t*)wbuf.bits;
                        memcpy(dst,rgb,outWidth*outHeight * 4);
                        ANativeWindow_unlockAndPost(nwin);
                    }
                }
            } else {
                //  音频
                uint8_t *out[2] = {0};
                out[0] = (uint8_t*) pcm;

                //音频重采样
                int len = swr_convert(actx,out,
                                      frame->nb_samples,
                                      (const uint8_t**)frame->data,
                                      frame->nb_samples);
                LOGW("swr_convert = %d", len);
            }
        }
    }

    delete rgb;
    delete pcm;

    // 关闭上下文
    avformat_close_input(&ic);
    env->ReleaseStringUTFChars(url_, path);
}