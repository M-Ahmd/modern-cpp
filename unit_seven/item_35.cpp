#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <stdexcept>

using namespace std;

// 1. الأسلوب المبني على المسارات (Thread-based - لا يفضله Scott Meyers)
int shared_result = 0;
mutex mtx; // نضطر لإنشاء قفل لحماية المتغير المشترك

void thread_worker() {
    try {
        // يجب إدارة القفل يدوياً لتخزين النتيجة
        lock_guard<mutex> lock(mtx);
        shared_result = 100;
    } catch (...) {
        // إذا حدث خطأ (Exception) ولم نلتقطه هنا، سينهار البرنامج بالكامل عبر std::terminate
    }
}

// 2. الأسلوب المبني على المهام (Task-based - الأسلوب المفضل)
int task_worker() {
    // لا نحتاج لأي أقفال (Mutex) أو متغيرات عامة، نرجع القيمة مباشرة
    throw runtime_error("Database connection lost!"); // محاكاة لحدوث خطأ
    return 200;
}

int main() {
    // --- تشغيل الأسلوب القديم ---
    thread t(thread_worker);
    t.join();
    cout << "Thread result: " << shared_result << "\n";

    // --- تشغيل الأسلوب الحديث (القاعدة 35) ---
    future<int> f = async(launch::async, task_worker);
    
    try {
        // دالة get() تقوم بأمرين: تجلب النتيجة النهائية، أو تعيد إطلاق الاستثناء بأمان لتلتقطه
        int result = f.get(); 
        cout << "Task result: " << result << "\n";
    } catch (const exception& e) {
        cout << "Task safely caught exception: " << e.what() << "\n";
    }

    return 0;
}
