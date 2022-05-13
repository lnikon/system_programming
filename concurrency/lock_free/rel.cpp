#include <atomic>
#include <mutex>
#include <thread>
#include <cassert>
#include <iostream>

std::mutex gCoutMutex;

void threadSafePrint(const std::string& msg)
{
    std::lock_guard<std::mutex> lg(gCoutMutex);
    std::cout << msg << std::endl;
}

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed));
    if (x.load(std::memory_order_relaxed))
    {
        ++z;
    }
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread c(write_x_then_y);
    std::thread d(read_y_then_x);
    c.join();
    d.join();
    assert(z.load() != 0);
    return 0;
}
