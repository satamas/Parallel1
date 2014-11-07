#include <iostream>
#include <future>
#include <map>
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

int main() {
    int id = 0;
    std::map<int, Future<int> *> tasks;
    ExecutorService executorService(1, std::chrono::milliseconds(500));
    while(true){
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
        } else if(command == "q"){
            exit(0);
        }
    }
}