#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

using namespace std;

// 1. الاستخدام الصحيح للتزامن: عدّاد معاملات آمن بين المسارات بدون Lock
atomic<int> db_transactions_count{0}; 

void process_queries() {
    for (int i = 0; i < 1000; ++i) {
        // هذه العملية الذرية آمنة 100% ولن يحدث Data Race
        db_transactions_count++; 
    }
}

// 2. الاستخدام الصحيح للعتاد (وهمي للتوضيح): التعامل مع مسجلات المتحكمات الدقيقة
volatile uint8_t* const hardware_register = reinterpret_cast<uint8_t*>(0x3F);

void send_lcd_command(uint8_t cmd) {
    // بدون volatile، المترجم الذكي سيحذف هذا السطر الأول لأنه يرى أن السطر الثاني سيكتب فوقه
    *hardware_register = cmd; 
    
    *hardware_register = 0x00; 
}

int main() {
    vector<thread> workers;
    for (int i = 0; i < 10; ++i) {
        workers.push_back(thread(process_queries));
    }

    for (auto& t : workers) {
        t.join();
    }

    cout << "Total Database Transactions: " << db_transactions_count << "\n";
    return 0;
}
