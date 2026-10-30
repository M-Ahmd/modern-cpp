#include <iostream>
#include <thread>
#include <stdexcept>

using namespace std;

// كلاس الحماية (ThreadRAII) لتطبيق القاعدة 37
class ThreadRAII {
public:
    enum class DtorAction { join, detach };

    // نستخدم دلالات النقل (Move Semantics) لأن المسارات لا يمكن نسخها
    ThreadRAII(thread&& t, DtorAction a) 
        : action(a), t(move(t)) {}

    ~ThreadRAII() {
        // حماية إضافية: لا نستدعي join أو detach إلا إذا كان المسار joinable
        if (t.joinable()) {
            if (action == DtorAction::join) {
                t.join();
            } else {
                t.detach();
            }
        }
    }

    thread& get() { return t; }

private:
    DtorAction action;
    thread t;
};

void process_data() {
    throw runtime_error("Buffer Pool limit reached!"); // محاكاة لانهيار مفاجئ
}

void bad_design() {
    thread t(process_data);
    // إذا رميت process_data استثناءً، لن نصل لسطر join وسينهار النظام عبر std::terminate
    t.join(); 
}

void good_design() {
    // نمرر المسار فوراً لكلاس الحماية ليضمن استدعاء join عند انتهاء النطاق
    ThreadRAII t(thread(process_data), ThreadRAII::DtorAction::join);

    // حتى مع حدوث الاستثناء وهروب التنفيذ من الدالة، 
    // سيقوم المُدمر الخاص بـ ThreadRAII باستدعاء join() بأمان لإنقاذ النظام.
}

int main() {
    try {
        good_design();
        // bad_design(); // جرب تشغيل هذا السطر لترى الانهيار العنيف بنفسك
    } catch(const exception& e) {
        cout << "Caught safely without crashing: " << e.what() << "\n";
    }
    return 0;
}
