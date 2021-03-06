//
// Created by Administrator on 2018/5/4 0004.
//

#ifndef CHAOPLAYER_CHAOTHREAD_H
#define CHAOPLAYER_CHAOTHREAD_H

//sleep 毫秒
void ChaoSleep(int mis);

//c++ 11 线程库
class ChaoThread {
public:
    //启动线程
    virtual bool Start();

    //通过控制isExit安全停止线程（不一定成功）
    virtual void Stop();

    // 暂停
    virtual void SetPause(bool isP);

    virtual bool IsPause() {
        isPausing = isPause;
        return isPause;
    }

    //入口主函数
    virtual void Main() {}

protected:
    bool isExit = false;
    bool isRuning = false;
    bool isPause = false;
    bool isPausing = false;//当前状态是否在暂停当中
private:
    void ThreadMain();
};


#endif //CHAOPLAYER_CHAOTHREAD_H
