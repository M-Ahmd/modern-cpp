#include <iostream>
#include <vector>

using namespace std;

// ========================================================
// تجهيزات (Dummy Definitions) لكي يعمل الكود وتختفي الأخطاء
// ========================================================

class Widget {};

// دالة ترجع vector<bool> (تُستخدم لتمثيل ميزات الـ Widget)
std::vector<bool> features(const Widget& w) {
    // نرجع مصفوفة مؤقتة حيث العنصر السادس (رقم 5) قيمته true
    return {false, false, false, false, false, true, false}; 
}

// دالة تقوم بمعالجة الـ Widget بناءً على الأولوية
void processWidget(const Widget& w, bool highPriority) {
    cout << "Processing widget. High priority: " << highPriority << endl;
}

// فئة وهمية لتمثيل المصفوفات (Matrices)
class Matrix {};

// دالة وهمية ترجع رقماً بدقة double
double calcEpsilon() {
    return 0.0001;
}

// ========================================================
// الدالة الرئيسية التي تحتوي على أمثلة البند السادس (Item 6)
// ========================================================

int main() {
    Widget w;

    // ---------------------------------------------------------
    // المثال الأول: كارثة الوكلاء المخفيين (Invisible Proxy Classes)
    // ---------------------------------------------------------

    // الطريقة الكلاسيكية (تعمل بنجاح، لأن التحويل إلى bool يتم فوراً)
    bool highPriority_old = features(w)[5]; 
    processWidget(w, highPriority_old);

    // الطريقة الكارثية باستخدام auto (Undefined Behavior):
    /*
    auto highPriority_bad = features(w)[5];
    processWidget(w, highPriority_bad); // ❌ قد ينهار البرنامج هنا!
    */
    // السبب: الدالة [] أرجعت "كائناً وكيلاً" (std::vector<bool>::reference)، 
    // و auto احتفظت به كما هو. هذا الكائن يحتوي على مؤشر يشير إلى الـ vector 
    // المؤقت الذي أرجعته دالة features، والذي تم تدميره بانتهاء السطر!

    // ---------------------------------------------------------
    // المثال الثاني: الحل السحري (Explicitly Typed Initializer)
    // ---------------------------------------------------------

    // ✅ الحل: نستخدم static_cast لإجبار التعبير على إرجاع bool قبل أن تستنتجه auto.
    auto highPriority_new = static_cast<bool>(features(w)[5]); 
    processWidget(w, highPriority_new); // الكود آمن تماماً!

    // مثال المصفوفات (الوكيل المخفي لعملية الجمع لتسريع الأداء):
    // Matrix m1, m2, m3, m4;
    // auto sum = static_cast<Matrix>(m1 + m2 + m3 + m4); 

    // ---------------------------------------------------------
    // المثال الثالث: إعلان النوايا بوضوح (Intentional Conversions)
    // ---------------------------------------------------------

    // أ) تقليل الدقة (من double إلى float):
    // float ep_old = calcEpsilon(); // الطريقة الكلاسيكية: تبدو كخطأ غير مقصود
    
    // ✅ الطريقة الاحترافية: رسالة واضحة للمبرمجين أن تقليل الدقة متعمد!
    auto ep_new = static_cast<float>(calcEpsilon()); 

    // ب) حساب مؤشر (من double إلى int):
    double d = 0.5; // نريد الوصول لمنتصف الحاوية
    std::vector<int> c = {10, 20, 30, 40, 50};
    
    // int index_old = d * c.size(); // تحويل ضمني خفي (Implicit)
    
    // ✅ الطريقة الاحترافية:
    auto index_new = static_cast<int>(d * c.size());
    cout << "Calculated Index: " << index_new << endl;

    return 0;
}