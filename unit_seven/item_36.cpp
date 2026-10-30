#include <iostream>
#include <future>
#include <thread>
#include <chrono>

using namespace std;

void heavy_db_task() {
    this_thread::sleep_for(chrono::seconds(1));
}

int main() {
    // 1. الفخ: الاعتماد على السلوك الافتراضي
    auto fut_default = async(heavy_db_task); 

    // إذا قرر النظام تشغيلها كـ "مؤجلة" (Deferred)، فإن دالة wait_for لن تقوم بتنفيذها.
    // ستُرجع دائماً حالة future_status::deferred، والشرط لن يتحقق أبداً، وسيتوقف البرنامج هنا للأبد!
    /*
    while (fut_default.wait_for(chrono::milliseconds(100)) != future_status::ready) {
        // حلقة لا نهائية إذا كانت المهمة مؤجلة
    }
    */

    // 2. الحل (القاعدة 36): إجبار النظام على التوازي الفعلي
    // استخدام std::launch::async يضمن إنشاء مسار جديد فوراً وعدم التأجيل مطلقاً
    auto fut_async = async(launch::async, heavy_db_task);

    cout << "Task is definitely running in the background...\n";

    // الآن هذه الحلقة آمنة 100%، لأننا متأكدون أن المهمة قيد التنفيذ الفعلي
    while (fut_async.wait_for(chrono::milliseconds(200)) != future_status::ready) {
        cout << "Waiting for DB task...\n";
    }
    
    cout << "Task completed safely!\n";

    return 0;
}
