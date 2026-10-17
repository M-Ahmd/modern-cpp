#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <string>

using namespace std;

// فئة وهمية (Dummy Class) لنستخدمها في أمثلة المؤشرات الذكية
class Widget {
public:
    int value;
    Widget(int v) : value(v) {}
    // تعريف معامل المقارنة لكي تعمل دالة اللامدا بشكل صحيح
    bool operator<(const Widget& other) const {
        return value < other.value;
    }
};

// ---------------------------------------------------------
// المثال الأول: مشكلة السطر المرعب في القوالب (Templates)
// ---------------------------------------------------------
template<typename It>
void dwim_old(It b, It e) {
    while (b != e) {
        // الطريقة القديمة (سيئة):
        // يجب استخدام iterator_traits لاستخراج نوع القيمة التي يشير إليها It
        // كود طويل ومزعج جداً للقراءة والكتابة.
        typename std::iterator_traits<It>::value_type currValue = *b;
        b++;
    }
}

template<typename It>
void dwim_new(It b, It e) {
    while (b != e) {
        // الطريقة الحديثة (ممتازة):
        // auto تستنتج النوع تلقائياً من القيمة التي يرجعها المؤشر (*b).
        auto currValue = *b;
        b++;
    }
}

int main() {
    // ---------------------------------------------------------
    // المثال الثاني: نسيان تهيئة المتغيرات (Uninitialized Variables)
    // ---------------------------------------------------------
    
    int x1; // الطريقة القديمة (سيئة): المتغير x1 قد يحتوي على قيمة عشوائية (Garbage).
    
    // auto x2; // ❌ خطأ ترجمة (Compile Error): auto تجبرك على إعطاء قيمة أولية.
    
    auto x3 = 0; // الطريقة الحديثة (ممتازة): المتغير x3 تم تهيئته بصفر وهو آمن تماماً.

    // ---------------------------------------------------------
    // المثال الثالث: تخزين دوال اللامدا (Closures)
    // ---------------------------------------------------------
    
    // الطريقة القديمة (سيئة): استخدام std::function
    // 1. كود طويل جداً ومكرر (يجب كتابة توقيع الدالة بالكامل).
    // 2. تستهلك مساحة أكبر في الذاكرة (قد تحجز من الـ Heap).
    // 3. أبطأ في التنفيذ لأنها تمنع المترجم من عمل Inlining.
    std::function<bool(const std::unique_ptr<Widget>&, const std::unique_ptr<Widget>&)>
        derefUPLess_old = [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2) {
            return *p1 < *p2;
        };

    // الطريقة الحديثة (ممتازة): استخدام auto
    // 1. كود قصير ونظيف.
    // 2. تحجز نفس المساحة الدقيقة التي تحتاجها اللامدا فقط.
    // 3. أسرع جداً في التنفيذ.
    auto derefUPLess_new = [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2) {
            return *p1 < *p2;
        };

    // ---------------------------------------------------------
    // المثال الرابع: فخ اختصارات الأنواع (Type Shortcuts) مع size()
    // ---------------------------------------------------------
    
    std::vector<int> v = {1, 2, 3};

    // الطريقة السيئة:
    // الدالة size() ترجع std::vector<int>::size_type وليس unsigned.
    // في أنظمة 64-bit، هذا المتغير سيقتطع البيانات لأنه 32-bit بينما الحجم الحقيقي 64-bit.
    unsigned sz_old = v.size(); 

    // الطريقة الممتازة (باستخدام auto):
    // auto ستأخذ النوع الدقيق (size_type) وتحمي البرنامج من أخطاء النقل بين 32-bit و 64-bit.
    auto sz_new = v.size();

    // ---------------------------------------------------------
    // المثال الخامس: الكارثة الخفية في حلقات for (Unintentional Type Mismatch)
    // ---------------------------------------------------------
    
    std::unordered_map<std::string, int> m;
    m["Apple"] = 10;
    
    // الطريقة السيئة:
    // المبرمج كتب std::pair<std::string, int> ولكن النوع الحقيقي داخل القاموس هو:
    // std::pair<const std::string, int> (المفتاح يجب أن يكون const).
    // هذا الاختلاف البسيط سيجعل المترجم يقوم بإنشاء "نسخة مؤقتة" لكل عنصر في القاموس،
    // مما يسبب بطئاً شديداً وعناوين ذاكرة وهمية.
    for (const std::pair<std::string, int>& p : m) {
        // يتم هنا التعامل مع نسخة مؤقتة يتم تدميرها في نهاية اللفة!
    }

    // الطريقة الممتازة (باستخدام auto):
    // المترجم سيستنتج النوع الدقيق const std::pair<const std::string, int>&
    // لا يوجد نسخ، الكود أسرع، ويشير إلى العنصر الأصلي في الذاكرة.
    for (const auto& p : m) {
        // يتم هنا التعامل مع العنصر الأصلي مباشرة.
    }

    return 0;
}