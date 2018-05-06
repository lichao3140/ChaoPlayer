//
// Created by Administrator on 2018/5/4 0004.
//

#include "ChaoDemux.h"
#include "ChaoLog.h"

void ChaoDemux::Main() {
    while (!isExit) {
        if(IsPause()) {
            ChaoSleep(2);
            continue;
        }
        ChaoData d = Read();
        if (d.size > 0)
            Notify(d);
        else
            ChaoSleep(2);
//        CHAOLOGI("ChaoDemux Read %d", d.size);
//        if (d.size <= 0) break;
    }
}
