#include <iostream>
#include <thread>
#include <future>
#include <chrono>

using namespace std;

// المسار العامل يستقبل future<void> كإشارة للبدء
void database_worker(future<void> init_signal) {
    cout << "[Worker] Waiting for database initialization...\n";
    
    // يتجمد المسار هنا كلياً دون استهلاك المعالج، ولا يحتاج لأي Lock
    init_signal.wait(); 
    
    cout << "[Worker] System is ready! Executing transactions.\n";
}

int main() {
    // إنشاء الوعد (الإرسال) والمستقبل (الاستقبال)
    promise<void> db_promise;
    future<void> db_future = db_promise.get_future();

    // تمرير الـ future للمسار عبر move لأنه غير قابل للنسخ
    thread worker(database_worker, move(db_future));

    cout << "[Main] Initializing Buffer Pool Manager and Disk Scheduler...\n";
    this_thread::sleep_for(chrono::seconds(2)); // محاكاة لعملية تهيئة معقدة
    cout << "[Main] Initialization complete.\n";

    // إرسال الإشارة عبر الوعد، مما يوقظ المسار العامل فوراً
    db_promise.set_value(); 

    worker.join();
    return 0;
}
