#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <iostream>
#include "FutureTask.h"

#pragma once
class Executor {
public:
    Executor();

    Executor(std::chrono::milliseconds timeout);

    void exec(Runnable * task);

    void onExecutionComplete(std::function<void(Executor *)> onComplete){
        _onComplete = onComplete;
    };

    bool isDead(){
        return _dead;
    }

private:
    void threadTask();

    std::function<void(Executor *)> _onComplete;
    std::thread _thread;
    std::mutex _execMutex;
    std::condition_variable _execCV;
    Runnable * _task = nullptr;
    bool _execReady = false;
    bool _dead = false;
    std::chrono::milliseconds _timeout;
    bool endless = false;
};