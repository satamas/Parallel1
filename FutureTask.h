#include <mutex>
#include <condition_variable>
#include "Future.h"
#include "Callable.h"
#include "Runnable.h"

#pragma once
template <class T>
class FutureTask: public Future<T>, public Runnable{
public:
    FutureTask(Callable<T> * callable):
            _resultMutex(){
        _callable = callable;
    }

    void run() override{
        std::unique_lock<std::mutex> lock(_resultMutex);
        result = _callable->call();
        done = true;
        _doneCV.notify_one();
    }

    T * get() override{
        std::unique_lock<std::mutex> lock(_resultMutex);
        while(!done && !isCanceled()){
            _doneCV.wait(lock);
        }
        if(isCanceled()){
            throw std::runtime_error("Task was canceled");
        }
        return result;
    }

    bool isDone() override{
        return done;
    }

    void cancel() override{
        if(!done) {
            _callable->canceled = true;
        }
    };

    bool isCanceled() override{
        return _callable->canceled;
    }
private:
    std::condition_variable _doneCV;
    std::mutex _resultMutex;
    Callable<T> * _callable = nullptr;
    T * result = nullptr;
    bool done = false;
};