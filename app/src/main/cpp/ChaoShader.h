//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOSHADER_H
#define CHAOPLAYER_CHAOSHADER_H

#include <mutex>

enum ChaoShaderType {
    XSHADER_YUV420P = 0,    //软解码和虚拟机
    XSHADER_NV12 = 25,      //手机
    XSHADER_NV21 = 26
};

class ChaoShader {
public:
    // 初始化
    virtual bool Init(ChaoShaderType type = XSHADER_YUV420P);

    virtual void Close();

    // 获取材质并映射到内存
    virtual void GetTexture(unsigned int index,int width,int height, unsigned char *buf, bool isa = false);

    // 绘制
    virtual void Draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0}; //设置材质  防止内存泄漏
    std::mutex mux;
};


#endif //CHAOPLAYER_CHAOSHADER_H
