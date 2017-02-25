#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// http://www.cnblogs.com/haippy/p/3252041.html
// C++11 并发指南五
// The process is produce one and then consume one.

std::mutex mtx;
std::condition_variable cv;

int cargo = 0;
bool shipment_available()
{
    return cargo != 0;
}

void consume(int n) // consume total n goods
{
    for (int i = 0; i < n; ++i) {
        std::unique_lock <std::mutex> lck(mtx);
        cv.wait(lck, shipment_available);
        std::cout << cargo << '\n';
        cargo = 0;
    }
}

int main()
{
    std::thread consumer_thread(consume, 10);

    // produce 10 goods
    for (int i = 0; i < 10; ++i) {
        while (shipment_available())
            std::this_thread::yield();
        std::unique_lock <std::mutex> lck(mtx);
        cargo = i + 1;
        cv.notify_one();
    }

    consumer_thread.join();

    return 0;
}