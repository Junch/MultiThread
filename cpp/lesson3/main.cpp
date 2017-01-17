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
       lock_guard<std::mutex> lockGuard(mu);
       ++sum;
    }
};

__declspec(noinline)
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
