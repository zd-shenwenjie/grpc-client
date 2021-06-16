#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <functional>

using namespace std;

class Timer
{
private:
    condition_variable cv;
    mutex mut;
    atomic<bool> expired;
    atomic<bool> tryToExpire;

public:
    Timer();
    ~Timer();
    void setTimeout(std::function<void()>, int delay);
    void setInterval(std::function<void()>, int interval);
    void stop();
};

#endif