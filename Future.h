#pragma once
template<class T>
class Future {
public:
    virtual T * get() = 0;

    virtual void cancel() = 0;

    virtual bool isCanceled() = 0;

    virtual bool isDone() = 0;

    virtual ~Future(){

    };
};