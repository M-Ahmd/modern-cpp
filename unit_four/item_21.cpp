#include <iostream>
#include <memory>

struct Widget {
    Widget() { std::cout << "Widget created\n"; }
    ~Widget() { std::cout << "Widget destroyed\n"; }
};

// دالة قد تفشل وتطلق استثناء (Exception)
int computePriority() {
    return 5;
}

void processWidget(std::shared_ptr<Widget> pw, int priority) {
    std::cout << "Processing Widget with priority " << priority << "\n";
}

// دالة حذف مخصصة
void customDeleter(Widget* w) {
    std::cout << "Custom deleting Widget...\n";
    delete w;
}

int main() {
    // 1. الطريقة القديمة (تحذير: خطر تسريب الذاكرة وبطيئة)
    // لو فشلت computePriority، سيبقى Widget معلقاً في الذاكرة
    // processWidget(std::shared_ptr<Widget>(new Widget), computePriority());

    // 2. الطريقة الذهبية (أمان تام للذاكرة + أداء أسرع)
    processWidget(std::make_shared<Widget>(), computePriority());

    // 3. الاستثناء الوحيد للقاعدة 
    std::shared_ptr<Widget> customWidget(new Widget, customDeleter);

    return 0;
}
