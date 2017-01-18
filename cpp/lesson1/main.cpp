#include <iostream>
#include <string>
#include <vector>
#include <thread>
using namespace std;

#ifdef _WIN32
__declspec(noinline)
#else
__attribute__ ((noinline))
#endif
void func(int &s) {
    s++;
}

// https://jyt0532.github.io/2016/12/23/c++-multi-thread-p2/

int main() {
    vector<thread> threads;
    int sum = 0;
    for (int i=0; i<1000; ++i) {
        threads.push_back(thread(func, ref(sum)));
    }

    for (thread& t: threads) {
        t.join();
    }

    cout << sum << endl;
    return 0;
}
