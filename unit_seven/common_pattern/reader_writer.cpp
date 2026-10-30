#include<unordered_map>
#include<iostream>
#include<shared_mutex>
#include<thread>
#include<vector>
#include<chrono>
#include<mutex>
#include<string>
using namespace std;

template<typename T, typename Q>
class ThreadSafeCache{
public:
    //writers
    void set_value(T key, Q value){
        unique_lock<shared_mutex> lock(mtx);
        database[key] = value;
    }
    //reader
    Q get_value(T key){
        
        shared_lock<shared_mutex> lock(mtx);
        auto it = database.find(key);
        if(it != database.end()){
            return it->second;
        }
        return Q{};
    }
private:
    unordered_map<T, Q> database;
    shared_mutex mtx;
};
int main() {
    ThreadSafeCache<int, string> cache;
    vector<thread> workers;

    workers.push_back(thread([&cache]() {
        cache.set_value(1, "BusTub");
        this_thread::sleep_for(chrono::milliseconds(50));
        cache.set_value(2, "C++17");
    }));

    for (int i = 0; i < 5; ++i) {
        workers.push_back(thread([&cache, i]() {
            this_thread::sleep_for(chrono::milliseconds(20)); 
            cout << "[Read Thread " << i << "] Key 1: " << cache.get_value(1) << "\n";
        }));
    }

    for (auto& t : workers) {
        t.join();
    }

    return 0;
}
