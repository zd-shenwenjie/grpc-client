#include "timer.h"

Timer::Timer()
{
    // cout << "Timer" << endl;
    _expired = true;
    _try_to_expire = false;
}

Timer::~Timer()
{
    // cout << "~Timer" << endl;
}

void Timer::setInterval(std::function<void()> function, int interval)
{
    if (!_expired)
        return;
    _expired = false;
    thread([this, interval, function]()
                {
                    while (!_try_to_expire)
                    {
                        this_thread::sleep_for(chrono::milliseconds(interval));
                        function();
                    }
                    lock_guard<std::mutex> locker(mut);
                    _expired = true;
                    cv.notify_one();
                })
        .detach();
}

void Timer::setTimeout(std::function<void()> function, int delay)
{
    thread([delay, function]()
                {
                    this_thread::sleep_for(chrono::milliseconds(delay));
                    function();
                })
        .detach();
}

void Timer::clearInterval()
{
    if (_expired)
        return;
    if (_try_to_expire)
        return;
    _try_to_expire = true;
    unique_lock<mutex> locker(mut);
    cv.wait(locker, [this]
            { return _expired == true; });
    if (_expired == true)
        _try_to_expire = false;
}