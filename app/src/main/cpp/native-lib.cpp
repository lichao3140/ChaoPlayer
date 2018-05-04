#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "ChaoPlayer", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "ChaoPlayer", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "ChaoPlayer", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "ChaoPlayer", __VA_ARGS__)

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

/*********************音频播放OpenSLES ****************************/
//  1.创建引擎
static SLObjectItf engineSL = NULL;
SLEngineItf CreateSL() {
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL, 0, 0, 0, 0, 0);// 创建引擎对象
    if(re != SL_RESULT_SUCCESS) // 失败
        return NULL;
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE); //成功   等待对象创建
    if(re != SL_RESULT_SUCCESS)
        return NULL;
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);  // 获取接口
    if(re != SL_RESULT_SUCCESS)
        return NULL;
    return en;
}

// 播放Pcm音频文件
void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *context) {
    LOGI("PcmCall");
    static FILE *fp = NULL;
    static char *buf = NULL;
    if(!buf) {
        buf = new char[1024*1024];
    }
    if(!fp) {
        fp = fopen("/sdcard/Music/test.pcm", "rb");
    }
    if(!fp)
        return;
    if(feof(fp) == 0) { // 打开成功
        int len = fread(buf, 1, 1024, fp);
        if(len > 0)
            (*bf)->Enqueue(bf, buf, len); // 发送音频
    }
}

//顶点着色器glsl
#define GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition; //顶点坐标
        attribute vec2 aTexCoord; //材质顶点坐标
        varying vec2 vTexCoord;   //输出的材质坐标
        void main(){
            vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
            gl_Position = aPosition; //显示顶点
        }
);

//片元着色器,软解码和部分x86硬解码
static const char *fragYUV420P = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main(){
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture,vTexCoord).r;
            yuv.g = texture2D(uTexture,vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture,vTexCoord).r - 0.5;
            rgb = mat3(1.0,     1.0,    1.0,
                       0.0,-0.39465,2.03211,
                       1.13983,-0.58060,0.0)*yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

GLint InitShader(const char *code, GLint type) {
    //创建shader
    GLint sh = glCreateShader(type);
    if(sh == 0) {
        LOGD("glCreateShader %d failed!", type);
        return 0;
    }
    //加载shader
    glShaderSource(sh,
                   1,    //shader数量
                   &code, //shader代码
                   0);   //代码长度
    //编译shader
    glCompileShader(sh);

    //获取编译情况
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if(status == 0) {
        LOGD("glCompileShader failed!");
        return 0;
    }
    LOGD("glCompileShader success!");
    return sh;
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

    //1 创建引擎
    SLEngineItf eng = CreateSL();
    if(eng){
        LOGD("CreateSL success！");
    }else{
        LOGD("CreateSL failed！");
    }

    //2 创建混音器
    SLObjectItf mix = NULL;
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);  // 1 引擎  2 输出的混音器 345配置项音效
    if (re != SL_RESULT_SUCCESS) {
        LOGD("SL_RESULT_SUCCESS failed!");
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE); // 实例化对象  1 对象  2 等待创建完毕
    if (re != SL_RESULT_SUCCESS) {
        LOGD("(*mix)->Realize failed!");
    }
    SLDataLocator_OutputMix outMix = {SL_DATALOCATOR_OUTPUTMIX, mix}; // 用来存放声音给音频
    SLDataSink audioSink= {&outMix, 0};

    //3 配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,10};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2,  // 声道数
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN // 字节序，小端
    };
    SLDataSource ds = {&que,&pcm};

    //4 创建播放器
    SLObjectItf player = NULL;
    SLPlayItf iplayer = NULL;
    SLAndroidSimpleBufferQueueItf pcmQue = NULL;  // 存放地址
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    // 创建播放器 1 引擎  2 播放器对象  3 数据源信息  4 混音器  5   6  7
    re = (*eng)->CreateAudioPlayer(eng, &player, &ds, &audioSink, sizeof(ids) / sizeof(SLInterfaceID), ids, req);
    if(re != SL_RESULT_SUCCESS) {
        LOGD("CreateAudioPlayer failed!");
    } else {
        LOGD("CreateAudioPlayer success!");
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE); // 实例化
    //获取player接口
    re = (*player)->GetInterface(player, SL_IID_PLAY,&iplayer);
    if(re != SL_RESULT_SUCCESS) {
        LOGD("GetInterface SL_IID_PLAY failed!");
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQue); // 获取播放用的接口
    if(re != SL_RESULT_SUCCESS) {
        LOGD("GetInterface SL_IID_BUFFERQUEUE failed!");
    }

    //设置回调函数，播放队列空调用
    (*pcmQue)->RegisterCallback(pcmQue, PcmCall, 0);

    //设置为播放状态
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PLAYING);

    //启动队列回调
    (*pcmQue)->Enqueue(pcmQue, "", 1);

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

extern "C"
JNIEXPORT void JNICALL
Java_com_lichao_chaoplayer_ChaoPlay_Yuv(JNIEnv *env, jobject instance, jstring url_, jobject surface) {
    const char *url = env->GetStringUTFChars(url_, 0);
    LOGD("open url is %s", url);
    FILE *fp = fopen(url, "rb");
    if(!fp) {
        LOGD("open file %s failed!", url);
        return;
    }

    //获取原始窗口
    ANativeWindow *nwin = ANativeWindow_fromSurface(env,surface);

    //1 EGL display创建和初始化
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);  //  创建
    if (display == EGL_NO_DISPLAY) {
        LOGD("eglGetDisplay failed!");
        return;
    }
    if (EGL_TRUE != eglInitialize(display, 0, 0)) { //  初始化
        LOGD("eglInitialize failed!");
        return;
    }

    //2 surface
    // 输出配置
    EGLConfig config;
    EGLint configNum;
    //输入配置
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE
    };
    // surface窗口配置
    if(EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &configNum)) {
        LOGD("eglChooseConfig failed!");
        return;
    }
    // 创建surface
    EGLSurface winSurface = eglCreateWindowSurface(display, config, nwin, 0);
    if(winSurface == EGL_NO_SURFACE) {
        LOGD("eglCreateWindowSurface failed!");
        return;
    }

    //3 context 创建关联的上下文
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
    if(context == EGL_NO_CONTEXT) {
        LOGD("eglCreateContext failed!");
        return;
    }
    if(EGL_TRUE != eglMakeCurrent(display, winSurface, winSurface, context)) {
        LOGD("eglMakeCurrent failed!");
        return;
    }
    LOGD("EGL Init Success!");

    //顶点和片元shader初始化
    //顶点shader初始化
    GLint vsh = InitShader(vertexShader, GL_VERTEX_SHADER);
    //片元yuv420 shader初始化
    GLint fsh = InitShader(fragYUV420P, GL_FRAGMENT_SHADER);

    //创建渲染程序
    GLint program = glCreateProgram();
    if(program == 0) {
        LOGD("glCreateProgram failed!");
        return;
    }
    //渲染程序中加入着色器代码
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    //链接程序
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program ,GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        LOGD("glLinkProgram failed!");
        return;
    }
    glUseProgram(program);// 激活渲染程序
    LOGD("glLinkProgram success!");

    /*****************************************************************/
    //加入三维顶点数据 两个三角形组成正方形
    static float vers[] = {
            1.0f,-1.0f,0.0f,
            -1.0f,-1.0f,0.0f,
            1.0f,1.0f,0.0f,
            -1.0f,1.0f,0.0f,
    };
    GLuint apos = (GLuint)glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(apos);
    //传递顶点
    glVertexAttribPointer(apos, 3, GL_FLOAT,GL_FALSE, 12, vers);

    //加入材质坐标数据
    static float txts[] = {
            1.0f,0.0f , //右下
            0.0f,0.0f,
            1.0f,1.0f,
            0.0,1.0
    };
    GLuint atex = (GLuint)glGetAttribLocation(program, "aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex, 2, GL_FLOAT,GL_FALSE, 8, txts);

    int width = 424;
    int height = 240;

    //材质纹理初始化
    //设置纹理层
    glUniform1i(glGetUniformLocation(program, "yTexture"), 0); //对于纹理第1层
    glUniform1i(glGetUniformLocation(program, "uTexture"), 1); //对于纹理第2层
    glUniform1i(glGetUniformLocation(program, "vTexture"), 2); //对于纹理第3层

    //创建opengl纹理
    GLuint texts[3] = {0};
    //创建三个纹理
    glGenTextures(3, texts);

    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[0]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,           //细节基本 0默认
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图
                 width, height, //拉升到全屏
                 0,             //边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL                    //纹理的数据
    );

    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[1]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,           //细节基本 0默认
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图
                 width/2, height/2, //拉升到全屏
                 0,             //边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL                    //纹理的数据
    );

    //设置纹理属性
    glBindTexture(GL_TEXTURE_2D, texts[2]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //设置纹理的格式和大小
    glTexImage2D(GL_TEXTURE_2D,
                 0,           //细节基本 0默认
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图
                 width/2, height/2, //拉升到全屏
                 0,             //边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图 要与上面一致
                 GL_UNSIGNED_BYTE, //像素的数据类型
                 NULL                    //纹理的数据
    );

    /*****************************************************************/
    //纹理的修改和显示
    unsigned char *buf[3] = {0};
    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];

    for(int i = 0; i < 10000; i++) {
        //420p   yyyyyyyy uu vv
        if(feof(fp) == 0) {
            fread(buf[0], 1, width*height, fp);
            fread(buf[1], 1, width*height / 4, fp);
            fread(buf[2], 1, width*height / 4, fp);
        }

        //激活第1层纹理,绑定到创建的opengl纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texts[0]);
        //替换纹理内容
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[0]);

        //激活第2层纹理,绑定到创建的opengl纹理
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texts[1]);
        //替换纹理内容
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[1]);

        //激活第2层纹理,绑定到创建的opengl纹理
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, texts[2]);
        //替换纹理内容
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[2]);

        //三维绘制
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //窗口显示
        eglSwapBuffers(display, winSurface);
    }

    env->ReleaseStringUTFChars(url_, url);
}