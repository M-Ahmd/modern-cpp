#include <iostream>

// =====================================================================
// ITEM 11: Prefer deleted functions (= delete) to private undefined ones
// =====================================================================

// ---------------------------------------------------------
// 1. PREVENTING COPYING (C++11 vs C++98)
// ---------------------------------------------------------
class ModernDevice {
public:
    ModernDevice() = default;

    // C++11 BEST PRACTICE: Declare as public and mark '= delete'
    // WHY? 
    // 1. Fails immediately at compile-time (even if called by friends).
    // 2. Clearer error message ("call to deleted function") instead of accessibility error.
    ModernDevice(const ModernDevice&) = delete;
    ModernDevice& operator=(const ModernDevice&) = delete;
};

class LegacyDevice {
private:
    // C++98 HACK: Declare as private and do not define. (DO NOT USE THIS NOW)
    // WHY it's bad? 
    // If a member/friend calls it, compilation passes, and it fails late at link-time.
    LegacyDevice(const LegacyDevice&);
    LegacyDevice& operator=(const LegacyDevice&);
};

// ---------------------------------------------------------
// 2. PREVENTING IMPLICIT CONVERSIONS
// ---------------------------------------------------------
bool isLucky(int number) {
    return number == 7;
}

// C++ allows implicit conversions (e.g., char/bool/float to int).
// We can use '= delete' on non-member functions to reject specific types.
bool isLucky(char) = delete;   // Rejects: isLucky('a');
bool isLucky(bool) = delete;   // Rejects: isLucky(true);
bool isLucky(double) = delete; // Rejects: both double and float (C++ prefers double conversion)

// ---------------------------------------------------------
// 3. DISABLING SPECIFIC TEMPLATE INSTANTIATIONS
// ---------------------------------------------------------
class PointerProcessor {
public:
    // Main template accepts any pointer type
    template<typename T>
    void processPointer(T* ptr) {
        std::cout << "Processing pointer\n";
    }
};

// C++11 SUPERPOWER: We can delete specific template specializations.
// NOTE: Must be done at namespace scope (outside the class).
// In C++98, we couldn't make these 'private' because specializations 
// must share the same access level as the primary template (which is public).
template<>
void PointerProcessor::processPointer<void>(void*) = delete; // Rejects void*

template<>
void PointerProcessor::processPointer<char>(char*) = delete; // Rejects char*


// ---------------------------------------------------------
// USAGE EXAMPLE (Uncommenting deleted lines will cause Compile-Time Errors)
// ---------------------------------------------------------
int main() {
    // 1. Copying
    ModernDevice d1;
    // ModernDevice d2 = d1; // ERROR: call to deleted constructor
    
    // 2. Implicit Conversions
    bool a = isLucky(10);    // OK
    // bool b = isLucky('x'); // ERROR: call to deleted function
    // bool c = isLucky(3.5f); // ERROR: call to deleted function
    
    // 3. Templates
    PointerProcessor p;
    int x = 5;
    p.processPointer(&x);     // OK (int*)
    
    void* voidPtr = nullptr;
    // p.processPointer(voidPtr); // ERROR: call to deleted function
    
    return 0;
}