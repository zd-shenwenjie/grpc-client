#include "timer.h"

Timer::Timer()
{
    _expired = true;
    _try_to_expire = false;
    cout << "Timer" << endl;
}

Timer::~Timer()
{
    cout << "~Timer" << endl;
}

void Timer::setInterval(std::function<void()> function, int interval)
{
    if (_expired == false)
        return;
    _expired = false;
    std::thread([this, interval, function]()
                {
                    while (!_try_to_expire)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                        function();
                    }
                    std::lock_guard<std::mutex> locker(mut);
                    _expired = true;
                    cv.notify_one();
                })
        .detach();
}

void Timer::setTimeout(std::function<void()> function, int delay)
{
    std::thread([delay, function]()
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
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
    std::unique_lock<std::mutex> locker(mut);
    cv.wait(locker, [this]
            { return _expired == true; });
    if (_expired == true)
        _try_to_expire = false;
}