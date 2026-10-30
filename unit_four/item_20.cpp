#include <iostream>
#include <memory>

struct B; // تصريح مبدئي (Forward declaration)

struct A {
    std::shared_ptr<B> bPtr; // A يمتلك B بقوة
    ~A() { std::cout << "A destroyed\n"; }
};

struct B {
    // نستخدم weak_ptr هنا لكسر الدائرة المغلقة!
    // لو استخدمنا shared_ptr هنا أيضاً، لن يتم تدمير الكائنين أبداً.
    std::weak_ptr<A> aPtr;
    ~B() { std::cout << "B destroyed\n"; }

    void checkA() {
        // نستخدم lock() لترقية المؤشر الضعيف وقراءة القيمة بأمان
        if (auto sharedA = aPtr.lock()) {
            std::cout << "A is still alive.\n";
        }
        else {
            std::cout << "A has been destroyed.\n";
        }
    }
};

int main() {
    {
        auto a = std::make_shared<A>(); // عداد A = 1
        auto b = std::make_shared<B>(); // عداد B = 1

        a->bPtr = b; // عداد B أصبح 2 (لأن A يمتلكه الآن)
        b->aPtr = a; // عداد A يبقى 1 (لأن B يراقبه فقط بمؤشر ضعيف)

        b->checkA(); // ستنجح الطباعة لأن A ما زال حياً

    } // هنا ينتهي النطاق (Scope) للمتغيرات a و b

    return 0;
}
