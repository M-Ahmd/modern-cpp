#include <iostream>
#include <thread>
#include <future>
#include <chrono>

using namespace std;

void heavy_db_cleanup() {
    this_thread::sleep_for(chrono::seconds(2));
    cout << "[Background] Cleanup finished.\n";
}

void test_thread_behavior() {
    thread t(heavy_db_cleanup);
    
    // لو خرجنا من الدالة الآن دون استدعاء t.join() أو t.detach()
    // سيقوم النظام باستدعاء std::terminate وسينهار البرنامج بالكامل (Segmentation Fault/Abort).
    t.join(); // إلزامي لتجنب الانهيار
}

void test_future_behavior() {
    {
        // نطلق المهمة في الخلفية
        auto fut = async(launch::async, heavy_db_cleanup);
        
        cout << "[Main] Leaving the scope...\n";
    } // هنا ينتهي عمر (fut). 
      // المُدمر (Destructor) الخاص بـ future لن ينهار، لكنه سيتوقف هنا (Block) 
      // لمدة ثانيتين ولن يكمل البرنامج حتى تنتهي heavy_db_cleanup!
      // هذا تجميد صامت قد يقتل أداء الـ Backend.

    cout << "[Main] This line will only print AFTER the 2 seconds.\n";
}

int main() {
    test_thread_behavior();
    test_future_behavior();
    return 0;
}
