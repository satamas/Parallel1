#include "ExecutorService.h"
#include <iostream>

ExecutorService::ExecutorService(int noOfHotThreads, std::chrono::milliseconds timeout):
_timeout(timeout){
    for(int i = 0; i < noOfHotThreads; ++i){
        addExecutor();
    }
}

void ExecutorService::addExecutor() {
    Executor * executor = new Executor();
    executor->onExecutionComplete([this](Executor * executor){
        std::unique_lock<std::mutex> lck(queueMutex);
        this->_executors.push(executor);
    });
    _executors.push(executor);
}

void ExecutorService::addExecutor(std::chrono::milliseconds timeout) {
    Executor * executor = new Executor(timeout);
    executor->onExecutionComplete([this](Executor * executor){
        std::unique_lock<std::mutex> lck(queueMutex);
        this->_executors.push(executor);
    });
    _executors.push(executor);
}
