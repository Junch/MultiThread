#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

struct Sum
{
    int sum = 0;
    mutex mu;

    void increase() {
       mu.lock();
       ++sum;
       mu.unlock();
    }
};

#ifdef _WIN32
__declspec(noinline)
#else
__attribute__ ((noinline))
#endif
void func(Sum &s) {
    s.increase();
}

int main() {
    vector<thread> threads;
    Sum s;
    for (int i=0; i<1000; ++i) {
        threads.push_back(thread(func, ref(s)));
    }

    for (thread& t: threads) {
        t.join();
    }

    cout << s.sum << endl;
    return 0;
}
