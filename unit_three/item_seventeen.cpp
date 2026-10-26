#include <iostream>
#include <vector>
#include <string>

// =====================================================================
// Item 17: Understand special member function generation
// =====================================================================

class LegacyWidget {
public:
    std::vector<int> data;

    // ❌ BAD: Just by declaring this destructor (even if empty), 
    // the compiler will SILENTLY disable the generation of Move operations.
    // std::move() will fall back to expensive copying!
    ~LegacyWidget() { 
        std::cout << "LegacyWidget destroyed\n"; 
    }
};

class ModernWidget {
public:
    std::vector<int> data;

    // ✅ GOOD: The Rule of Five
    // If you declare ANY of these 5 functions, declare ALL of them explicitly.

    // 1. Destructor
    ~ModernWidget() = default;

    // 2. Move Constructor (Fast!)
    // We explicitly tell the compiler: "Please generate this for me!"
    ModernWidget(ModernWidget&&) = default;

    // 3. Move Assignment Operator (Fast!)
    ModernWidget& operator=(ModernWidget&&) = default;

    // 4. Copy Constructor (Fallback/Safe)
    ModernWidget(const ModernWidget&) = default;

    // 5. Copy Assignment Operator (Fallback/Safe)
    ModernWidget& operator=(const ModernWidget&) = default;
};

int main() {
    LegacyWidget lw1;
    // This will COPY the data, not move it, because Move ops are disabled!
    LegacyWidget lw2 = std::move(lw1); 

    ModernWidget mw1;
    // This will perfectly MOVE the data, because we explicitly defaulted it.
    ModernWidget mw2 = std::move(mw1); 

    return 0;
}