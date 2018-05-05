//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoTexture.h"
#include "ChaoLog.h"
#include "ChaoEGL.h"
#include "ChaoShader.h"

class CChaoTexture:public ChaoTexture {
public:
    ChaoShader sh;
    ChaoTextureType type;

    virtual bool Init(void *win, ChaoTextureType type) {
        this->type = type;
        if(!win) {
            CHAOLOGE("CChaoTexture Init failed win is NULL");
            return false;
        }
        // 传入窗口
        if(!ChaoEGL::Get()->Init(win))return false;
        sh.Init((ChaoShaderType)type);
        return true;
    }

    virtual void Draw(unsigned char *data[], int width, int height) {
        sh.GetTexture(0, width, height, data[0]);  // Y
        if(type == XTEXTURE_YUV420P) {
            sh.GetTexture(1, width/2, height/2, data[1]);  // U
            sh.GetTexture(2, width/2, height/2, data[2]);  // V
        } else {
            sh.GetTexture(1, width/2, height/2, data[1], true);  // UV
        }
        sh.Draw();
        ChaoEGL::Get()->Draw();
    }
};

ChaoTexture *ChaoTexture::Create() {
    return  new CChaoTexture();
}