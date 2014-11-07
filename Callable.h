#pragma once

class interrupted_exception: std::exception{
public:
    interrupted_exception(){}
};

template <class T>
class Callable{
public:
    virtual T * call() = 0;

protected:
    virtual bool onCanceled(){}

    void checkIfCanceled(){
        if(canceled){
            onCanceled();
            throw interrupted_exception();
        }
    }
private:
    template <class U>
    friend class FutureTask;

    bool canceled;
};