#include <iostream>
#include <vector>
#include <string>
#include <utility> // for std::move

// ========================================================
// 1. CLASS WITHOUT noexcept (The Vector will NOT trust it)
// ========================================================
class LegacyData {
public:
    std::string name;

    LegacyData(std::string n) : name(n) {}

    // Copy Constructor: Safe but slow.
    LegacyData(const LegacyData& other) {
        name = other.name;
        std::cout << "   [LegacyData] COPIED (Slow) - because no 'noexcept' guarantee!\n";
    }

    // Move Constructor: Fast, but MISSING 'noexcept'!
    // The compiler thinks this might throw an exception.
    LegacyData(LegacyData&& other) { 
        name = std::move(other.name);
        std::cout << "   [LegacyData] MOVED (Fast)\n";
    }
};

// ========================================================
// 2. CLASS WITH noexcept (The Vector trusts it 100%)
// ========================================================
class ModernData {
public:
    std::string name;

    ModernData(std::string n) : name(n) {}

    // Copy Constructor: Safe but slow.
    ModernData(const ModernData& other) {
        name = other.name;
        std::cout << "   [ModernData] COPIED (Slow)\n";
    }

    // Move Constructor: Fast and GUARANTEED safe!
    // Notice the 'noexcept' keyword here.
    ModernData(ModernData&& other) noexcept { 
        name = std::move(other.name);
        std::cout << "   [ModernData] MOVED (Fast) - Vector trusts the 'noexcept' guarantee!\n";
    }

    // Best Practice: Also mark custom swap as noexcept
    friend void swap(ModernData& a, ModernData& b) noexcept {
        std::swap(a.name, b.name);
    }
};

int main() {
    // ---------------------------------------------------------
    // TEST 1: What happens without noexcept?
    // ---------------------------------------------------------
    std::cout << "--- Testing LegacyData (NO noexcept) ---\n";
    std::vector<LegacyData> legacy_vec;
    legacy_vec.reserve(1); // Set maximum capacity to 1
    legacy_vec.push_back(LegacyData("Item 1")); 
    
    std::cout << "Adding Item 2 (Vector is full, must reallocate!):\n";
    // Result: The vector copies the old element because it fears losing data if the move fails.
    legacy_vec.push_back(LegacyData("Item 2")); 

    std::cout << "\n==========================================\n\n";

    // ---------------------------------------------------------
    // TEST 2: What happens with noexcept?
    // ---------------------------------------------------------
    std::cout << "--- Testing ModernData (WITH noexcept) ---\n";
    std::vector<ModernData> modern_vec;
    modern_vec.reserve(1); // Set maximum capacity to 1
    modern_vec.push_back(ModernData("Item 1")); 

    std::cout << "Adding Item 2 (Vector is full, must reallocate!):\n";
    // Result: The vector boldly moves the old element because 'noexcept' acts as an unbreakable promise.
    modern_vec.push_back(ModernData("Item 2"));

    return 0;
}