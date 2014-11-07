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
        while (!_availableExecutors.empty() && _availableExecutors.front()->isDead()) _availableExecutors.pop();

        if(_availableExecutors.empty()) addExecutor(_timeout);

        Executor * _executor = _availableExecutors.front();
        _availableExecutors.pop();
        _executor->exec(task);

        return task;
    }

    ~ExecutorService(){
        for(auto executor : _executros){
            delete executor;
        }
    }
private:
    void addExecutor();
    void addExecutor(std::chrono::milliseconds timeout);
    std::mutex queueMutex;
    std::chrono::milliseconds _timeout;
    std::queue<Executor *> _availableExecutors;
    std::vector<Executor *> _executros;

};