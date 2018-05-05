//
// Created by Administrator on 2018/5/5 0005.
//

#include "ChaoResample.h"
#include "ChaoLog.h"

void ChaoResample::Update(ChaoData data) {
    ChaoData d = this->Resample(data);
    //CHAOLOGE("his->Resample(data) %d", d.size);
    if(d.size > 0) {
        this->Notify(d);
    }
}