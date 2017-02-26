#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;

// http://www.cnblogs.com/haippy/p/3252041.html

int cargo = 0; // shared value by producers and consumers

void consumer()
{
    // Uncomment the line below, the app will enter dead lock.
    // This app has a bug. Before consumer unload the cargo, the cargo is
    // updated by other producers.
    // std::this_thread::sleep_for((std::chrono::milliseconds(1000))); 
    std::unique_lock < std::mutex > lck(mtx); 
    while (cargo == 0)
        cv.wait(lck);
    std::cout << cargo << '\n';
    cargo = 0;
}

void producer(int id)
{
    std::unique_lock < std::mutex > lck(mtx);
    cargo = id;
    cv.notify_one();
}

int main()
{
    std::thread consumers[10], producers[10];

    // spawn 10 consumers and 10 producers:
    for (int i = 0; i < 10; ++i) {
        consumers[i] = std::thread(consumer);
        producers[i] = std::thread(producer, i + 1);
    }

    // join them back:
    for (int i = 0; i < 10; ++i) {
        producers[i].join();
        consumers[i].join();
    }

    return 0;
}