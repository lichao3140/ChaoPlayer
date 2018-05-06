//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOTEXTURE_H
#define CHAOPLAYER_CHAOTEXTURE_H

enum ChaoTextureType {
    XTEXTURE_YUV420P = 0,  // Y 4  u 1 v 1
    XTEXTURE_NV12 = 25,    // Y4   uv1
    XTEXTURE_NV21 = 26     // Y4   vu1
};

class ChaoTexture {
public:

    static ChaoTexture *Create();

    virtual bool Init(void *win, ChaoTextureType type = XTEXTURE_YUV420P) = 0;

    virtual void Draw(unsigned char *data[], int width, int height) = 0;

    virtual void Drop() = 0;

    virtual ~ChaoTexture(){};
protected:
    ChaoTexture(){};
};


#endif //CHAOPLAYER_CHAOTEXTURE_H
