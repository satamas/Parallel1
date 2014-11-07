#pragma once
#include <chrono>
#include <queue>
#include "Executor.h"
#include "Callable.h"
#include "FutureTask.h"

class ExecutorService{
public:
    ExecutorService(int noOfHotThreads, std::chrono::milliseconds timeout);

    template <class T>
    Future<T> * execute(Callable<T> * callable){
        FutureTask<T> * task = new FutureTask<T>(callable);

        std::unique_lock<std::mutex> lck(queueMutex);
        while (!_executors.empty() && _executors.front()->isDead()) _executors.pop();

        if(_executors.empty()) addExecutor(_timeout);

        Executor * _executor = _executors.front();
        _executors.pop();
        _executor->exec(task);

        return task;
    }
private:
    void addExecutor();
    void addExecutor(std::chrono::milliseconds timeout);
    std::mutex queueMutex;
    std::chrono::milliseconds _timeout;
    std::queue<Executor *> _executors;
};