//
// Created by Administrator on 2018/5/5 0005.
//

#ifndef CHAOPLAYER_CHAOTEXTURE_H
#define CHAOPLAYER_CHAOTEXTURE_H


class ChaoTexture {
public:

    static ChaoTexture *Create();

    virtual bool Init(void *win) = 0;

    virtual void Draw(unsigned char *data[], int width, int height) = 0;
};


#endif //CHAOPLAYER_CHAOTEXTURE_H
