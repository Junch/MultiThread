#include <thread> 
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>
#include <deque>
using namespace std;

struct QueueBuffer {
    deque<int> deq;
    int capacity;
    mutex lock;
    condition_variable not_full;
    condition_variable not_empty;

    QueueBuffer(int capacity) : capacity(capacity){}

    void deposit(int data){
        unique_lock<mutex> lk(lock);
        while(deq.size() == capacity){
            not_full.wait(lk);    
        }
        deq.push_back(data);
        lk.unlock();
        not_empty.notify_one();
    }

    int fetch(){
        unique_lock<mutex> lk(lock);
        while(deq.size() == 0){
            not_empty.wait(lk);
        }
        int ret = deq.front();
        deq.pop_front();
        lk.unlock();
        not_full.notify_one();
        return ret;
    }
};

void consumer(int id, QueueBuffer& buffer){
    for(int i = 0; i < 20; ++i){
        int value = buffer.fetch();
        std::stringstream msg;
        msg << "Consumer " << id << " fetched " << value << endl;
        std::cout << msg.str();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void producer(int id, QueueBuffer& buffer){
    for(int i = 0; i < 30; ++i){
        buffer.deposit(i);
        std::stringstream msg;
        msg << "Producer " << id << " produced " << i << endl;
        std::cout << msg.str();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main(){
    QueueBuffer buffer(4);

    thread c1(consumer, 0, ref(buffer));
    thread c2(consumer, 1, ref(buffer));
    thread c3(consumer, 2, ref(buffer));
    thread p1(producer, 0, ref(buffer));
    thread p2(producer, 1, ref(buffer));

    c1.join();
    c2.join();
    c3.join();
    p1.join();
    p2.join();

    return 0;
}
