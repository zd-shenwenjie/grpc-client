#include "timer.h"

Timer::Timer()
{
    expired = true;
    tryToExpire = false;
    cout << "Timer" << endl;
}

Timer::~Timer()
{
    cout << "~Timer" << endl;
}

void Timer::setInterval(std::function<void()> function, int interval)
{
    if (expired == false) return;
    expired = false;
    std::thread([this, interval, function]() {
        while (!tryToExpire) {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            function(); 
        }
        std::lock_guard<std::mutex> locker(mut);
        expired = true;
        cv.notify_one();
    }).detach();
}

void Timer::setTimeout(std::function<void()> function, int delay)
{
    std::thread([delay, function]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		function();
	}).detach();
}

void Timer::stop()
{
    if (expired) return;
    if (tryToExpire) return;
    tryToExpire = true;
    std::unique_lock<std::mutex> locker(mut);
    cv.wait(locker, [this] { 
        return expired == true; 
    });
    if (expired == true) tryToExpire = false;
}