//
// Created by Administrator on 2018/5/4 0004.
//

#include "ChaoThread.h"
#include "ChaoLog.h"

#include <thread>
using namespace std;

void ChaoSleep(int mis) {
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

void ChaoThread::SetPause(bool isP) {
    isPause = isP;
    //等待100毫秒
    for(int i = 0; i < 10; i++) {
        if(isPausing == isP) {
            break;
        }
        ChaoSleep(10);
    }
}

//启动线程
bool ChaoThread::Start() {
    isExit = false;
    isPause = false;
    thread th(&ChaoThread::ThreadMain, this);
    th.detach();
    return true;
}

//通过控制isExit安全停止线程（不一定成功）
void ChaoThread::Stop() {
    CHAOLOGI("Stop 停止线程begin!");
    isExit = true;
    for(int i = 0; i < 200; i++) {
        if(!isRuning) {
            CHAOLOGI("Stop 停止线程成功!");
            return;
        }
        ChaoSleep(1);
    }
    CHAOLOGI("Stop 停止线程超时!");
}

void ChaoThread::ThreadMain() {
    isRuning = true;
    CHAOLOGI("线程函数进入");
    Main();
    CHAOLOGI("线程函数退出");
    isRuning = false;
}