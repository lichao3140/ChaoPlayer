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
    virtual void Start();

    //通过控制isExit安全停止线程（不一定成功）
    virtual void Stop();

    //入口主函数
    virtual void Main() {}

protected:
    bool isExit = false;
    bool isRuning = false;
private:
    void ThreadMain();
};


#endif //CHAOPLAYER_CHAOTHREAD_H
