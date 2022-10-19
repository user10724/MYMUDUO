#pragma once
#include "noncopyable.h"
#include <functional>
#include <vector>
#include <atomic>
#include "Timestamp.h"
#include <memory>
#include <mutex>
#include "CurrentThread.h"

class Channel;
class Poller;

//时间循环类 主要包含2个大模块Channel Poller(epoll的抽象)
class EventLoop
{

public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();
    //开启事件循环
    void loop();
    //退出事件循环
    void quit();

    Timestamp pollReturnTime() const{
        return pollReturnTime_;
    }

    //在当前loop中执行cb
    void runInLoop(Functor cb);
    //把cb放在队列中，唤醒loop所在的线程，执行cb
    void queueInLoop(Functor cb);
    //用来唤醒loop所在的线程的
    void wakeup();

    //EventLoop的方法 =》 poller的方法
    void updateChannel(Channel* channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel * channel);

    //判断EventLoop对象是否在自己的线程里面
    bool isInLoopThread()const{
        return threadId_==CurrentThread::tid();
    }

private:
    void handleRead();  //wakeup
    void doPendignFunctors();   //回调

    using ChannelList =std::vector<Channel*> ;

    std::atomic_bool looping_;  //原子操作，通过CAS实现的
    std::atomic_bool quit_;     //标识推出loop循环
   

    const pid_t threadId_;//记录当前loop所在线程的id
    Timestamp pollReturnTime_;  //poller返回事件的channels的时间点
    std::unique_ptr<Poller> poller_;

    int wakeupFd_;  //主要作用，当mainLoop获取一个新用户的channel,通过轮询算法选择一个subloop
                    //通过改成员唤醒subloop处理channel
    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;
    //Channel *currentActiveChannel_;

    std::atomic_bool callingPendingFuncors_;    //标识当前loop是否有需要执行的回调操作
    std::vector<Functor> pendingFunctors_;         //存储loop徐执行的所有回调操作
    std::mutex mutex_;      //互斥锁，用来保护上面vector容器的线程安全操作
};

