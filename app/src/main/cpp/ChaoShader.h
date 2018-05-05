//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOSHADER_H
#define CHAOPLAYER_CHAOSHADER_H


class ChaoShader {
public:
    // 初始化
    virtual bool Init();

    // 获取材质并映射到内存
    virtual void GetTexture(unsigned int index,int width,int height, unsigned char *buf);

    // 绘制
    virtual void Draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0}; //设置材质  防止内存泄漏
};


#endif //CHAOPLAYER_CHAOSHADER_H
