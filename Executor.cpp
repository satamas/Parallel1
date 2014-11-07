#include <iostream>
#include "Executor.h"

Executor::Executor() :
        _execMutex(),
        _thread(&Executor::threadTask, std::ref(*this)) {
    endless = true;
    _thread.detach();
}

Executor::Executor(std::chrono::milliseconds timeout)  :
        _execMutex(),
        _thread(&Executor::threadTask, std::ref(*this)),
        _timeout(timeout) {
    _thread.detach();
}

void Executor::exec(Runnable *task){
        std::unique_lock<std::mutex> lck(_execMutex);
        while (_execReady) _execCV.wait(lck);
        _task = task;
        _execReady = true;
        _execCV.notify_one();
}

void Executor::threadTask()  {
    while (true) {
        std::unique_lock<std::mutex> lck(_execMutex);

        if (endless) {
            _execCV.wait(lck, [this]() {
                return this->_execReady;
            });
        } else {
            std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
            std::chrono::system_clock::time_point end = start + _timeout;

            _execCV.wait_for(lck, std::chrono::duration_cast<std::chrono::milliseconds>(end - std::chrono::system_clock::now()), [this]() {
                return this->_execReady;
            });

            if (!_execReady) {
                break;
            }
        }
        try {
            _task->run();
        } catch (...){}
        std::cout << (bool) _onComplete;
        if((bool) _onComplete) {
            _onComplete(this);
        }
        _execReady = false;
        _execCV.notify_one();
    }
    _dead = true;
};
