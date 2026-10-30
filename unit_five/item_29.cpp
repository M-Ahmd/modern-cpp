#include <iostream>
#include <vector>
#include <array>
#include <utility>

// -------------------------------------------------------------
// 1. غياب noexcept: الـ std::vector يتراجع بصمت إلى الـ Copy
// -------------------------------------------------------------
class SlowFrame {
public:
    SlowFrame() = default;
    
    // Copy Constructor
    SlowFrame(const SlowFrame&) {
        std::cout << "SlowFrame: Deep Copy invoked!\n";
    }
    
    // Move Constructor بدون noexcept:
    // الـ vector لن يثق به خوفاً من حدوث Exception يفسد البيانات
    SlowFrame(SlowFrame&&) {
        std::cout << "SlowFrame: Move invoked!\n";
    }
};

class FastFrame {
public:
    FastFrame() = default;
    
    FastFrame(const FastFrame&) {
        std::cout << "FastFrame: Deep Copy invoked!\n";
    }
    
    // Move Constructor مع noexcept:
    // يضمن للـ vector استدعاء الـ Move بأمان عند إعادة التحجيم
    FastFrame(FastFrame&&) noexcept {
        std::cout << "FastFrame: Move invoked!\n";
    }
};

// -------------------------------------------------------------
// 2. الفرق بين تخزين الـ Heap (رخيص) وتخزين الـ Stack (مكلف)
// -------------------------------------------------------------
void DemonstrateStackVsHeapMove() {
    // std::vector:
    // يملك مؤشراً فقط في الـ Stack، بينما البيانات الفعلية في الـ Heap.
    // الـ Move هنا O(1): تبديل مؤشرات (Pointer Swap) فقط.
    std::vector<int> heap_vec(1'000'000, 7);
    auto moved_vec = std::move(heap_vec); // سريع جداً (لا يتم نسخ المليون عنصر)

    // std::array:
    // البيانات بالكامل مخزنة داخل الكائن نفسه في الـ Stack.
    // الـ Move هنا O(N): المترجم مجبر على نسخ الذاكرة بايت ببايت.
    std::array<int, 1'000'000> stack_arr{};
    auto moved_arr = std::move(stack_arr); // بطيء (تكلفة الـ Move = تكلفة الـ Copy)
}

// -------------------------------------------------------------
// 3. الأنواع الأساسية (Built-in Types) لا تملك Move أصلاً
// -------------------------------------------------------------
void DemonstrateBuiltinTypes() {
    int page_id = 42;
    
    // int لا يملك مؤشرات داخلية؛ std::move هنا لا تفعل أي شيء سوى النسخ العادي
    int new_page_id = std::move(page_id); // مجرد Copy لـ 4 بايت
    
    // page_id يظل محتفظاً بقيمته الأصلية 42 ولن يتم تفريغه
    std::cout << "page_id is still: " << page_id << "\n";
}

int main() {
    std::cout << "--- 1. Testing vector reallocation without noexcept ---\n";
    std::vector<SlowFrame> slow_vector;
    slow_vector.emplace_back(); // سعة 1
    std::cout << "Triggering reallocation:\n";
    slow_vector.emplace_back(); // سعة 2 (سيضطر لنقل العنصر الأول) -> سينسخ!

    std::cout << "\n--- 2. Testing vector reallocation with noexcept ---\n";
    std::vector<FastFrame> fast_vector;
    fast_vector.emplace_back();
    std::cout << "Triggering reallocation:\n";
    fast_vector.emplace_back(); // سينقل (Move) بدلاً من النسخ!

    std::cout << "\n--- 3. Testing Built-in Types ---\n";
    DemonstrateBuiltinTypes();

    return 0;
}
