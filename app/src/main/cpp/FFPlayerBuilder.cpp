//
// Created by Administrator on 2018/5/5 0005.
//

#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFdecode.h"
#include "FFResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"

ChaoDemux *FFPlayerBuilder::CreateDemux() {
    ChaoDemux *ff = new FFDemux();
    return ff;
}

ChaoDecode *FFPlayerBuilder::CreateDecode() {
    ChaoDecode *ff = new FFDecode();
    return ff;
}

ChaoResample *FFPlayerBuilder::CreateResample() {
    ChaoResample *ff = new FFResample();
    return ff;
}

ChaoVideoView *FFPlayerBuilder::CreateVideoView() {
    ChaoVideoView *ff = new GLVideoView();
    return ff;
}

ChaoAudioPlay *FFPlayerBuilder::CreateAudioPlay() {
    ChaoAudioPlay *ff = new SLAudioPlay();
    return ff;
}

ChaoPlayer *FFPlayerBuilder::CreatePlayer(unsigned char index) {
    return ChaoPlayer::Get(index);
}

void FFPlayerBuilder::InitHard(void *vm) {
    FFDecode::InitHard(vm);
}
