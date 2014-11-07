#include <iostream>
#include <future>
#include <map>
#include <signal.h>
#include "Executor.h"
#include "ExecutorService.h"

using namespace std;

class MyCallable : public Callable<int> {
public:
    MyCallable(int id, int time) {
        _id = id;
        _time = time;
    }

    int * call() override {
        std::cout << "Task " << _id << " started." << std::endl;
        for(int timer = 0; timer < _time; ++ timer){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            checkIfCanceled();
        }
        std::cout << "Task " << _id << " ended." << std::endl;
        return nullptr;
    }

private:
    int _id;
    int _time;
};

bool terminated;
void my_handler(int s){
    if(s == SIGTERM) {
        terminated = true;
        exit(1);
    }
}


int main() {
    int id = 0;
    std::map<int, Future<int> *> tasks;
    ExecutorService executorService(1, std::chrono::milliseconds(500));

    struct sigaction sigTermHandler;
    sigTermHandler.sa_handler = my_handler;
    sigemptyset(&sigTermHandler.sa_mask);
    sigTermHandler.sa_flags = 0;
    sigaction(SIGTERM, &sigTermHandler, NULL);

    while(!terminated){
        std::string command;
        std::cin >> command;
        if(command == "add"){
            int taskTime;
            std::cin >> taskTime;
            std::cout << id << std::endl;
            tasks[id] = executorService.execute(new MyCallable(id, taskTime));
            ++id;
        } else if(command == "cancel"){
            int taskId;
            std::cin >> taskId;
            tasks[taskId]->cancel();
        }
    }

    for(auto it = tasks.begin(); it != tasks.end(); ++it){
        it->second->cancel();
    }
}