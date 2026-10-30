#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

struct Account {
    int balance;
    mutex mtx;
    Account(int b) : balance(b) {}
};

void transfer(Account &from, Account &to, int amount) {
    // المطلوب: اكتب الكود هنا لخصم المبلغ من from وإضافته لـ to
    // يجب استخدام std::scoped_lock لقفل الحسابين بأمان
    scoped_lock lock(from.mtx, to.mtx);
    from.balance -= amount;
    to.balance += amount;
}

int main() {
    Account acc1(1000);
    Account acc2(500);

    // مساران يحولان الأموال في اتجاهات متعاكسة في نفس اللحظة
    thread t1(transfer, ref(acc1), ref(acc2), 200);
    thread t2(transfer, ref(acc2), ref(acc1), 100);

    t1.join();
    t2.join();

    // النتيجة المتوقعة: Acc1 = 900, Acc2 = 600
    cout << "Acc1: " << acc1.balance << " | Acc2: " << acc2.balance << "\n";
    
    return 0;
}
