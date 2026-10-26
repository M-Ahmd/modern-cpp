#include <iostream>
#include <array>

// =====================================================================
// PART 1: The Evolution of constexpr Functions (C++11 vs C++14)
// =====================================================================

// C++11 Approach: Strict limitations.
// In C++11, a constexpr function could only have ONE return statement.
// We had to use recursion and the ternary operator (?:) for logic.
constexpr int pow_cpp11(int base, int exp) noexcept {
    return (exp == 0 ? 1 : base * pow_cpp11(base, exp - 1));
}

// C++14 Approach: Relaxed rules.
// In C++14, we can use local variables, loops (for/while), and normal if-statements.
// This makes the code much cleaner and easier to read.
constexpr int pow_cpp14(int base, int exp) noexcept {
    auto result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

// =====================================================================
// PART 2: Literal Types and User-Defined Classes with constexpr
// =====================================================================

class Point {
public:
    // 1. constexpr Constructor: Allows creating Point objects at compile-time.
    // 'noexcept' is used because compile-time initialization shouldn't throw.
    constexpr Point(double xVal = 0, double yVal = 0) noexcept 
        : x(xVal), y(yVal) {}

    // 2. constexpr Getters: Allow reading values at compile-time.
    constexpr double xValue() const noexcept { return x; }
    constexpr double yValue() const noexcept { return y; }

    // 3. constexpr Setters: (Only possible in C++14 and later!)
    // In C++11, constexpr member functions were implicitly const, so they couldn't modify 'x' or 'y'.
    // C++14 lifted this restriction, allowing us to modify objects during compile-time.
    constexpr void setX(double newX) noexcept { x = newX; }
    constexpr void setY(double newY) noexcept { y = newY; }

private:
    double x, y;
};

// =====================================================================
// PART 3: Compile-Time Computations with User-Defined Types
// =====================================================================

// A constexpr function that takes Points, creates a new Point, modifies it, and returns it.
// ALL of this happens during compilation if the inputs are known!
constexpr Point reflection(const Point& p) noexcept {
    Point result;                  // Create a local object
    result.setX(-p.xValue());      // Use the constexpr setter (C++14 feature)
    result.setY(-p.yValue());
    return result;                 // Return the modified object
}

int main() {
    // -----------------------------------------------------------------
    // Example A: Using constexpr functions in contexts that REQUIRE compile-time constants.
    // -----------------------------------------------------------------
    constexpr int numConds = 5;
    
    // The size of std::array MUST be a compile-time constant.
    // The compiler computes pow_cpp14(3, 5) -> 243 during compilation.
    std::array<int, pow_cpp14(3, numConds)> results; 
    std::cout << "Array size (computed at compile-time): " << results.size() << "\n";

    // -----------------------------------------------------------------
    // Example B: Compile-time object creation and modification.
    // -----------------------------------------------------------------
    constexpr Point p1(9.4, 27.7); // Object created in read-only memory during compilation.
    
    // The compiler runs the reflection() function internally.
    // The final result (-9.4, -27.7) is hardcoded into the binary. No runtime cost!
    constexpr auto reflected_p1 = reflection(p1); 

    std::cout << "Reflected Point: (" << reflected_p1.xValue() << ", " << reflected_p1.yValue() << ")\n";

    // -----------------------------------------------------------------
    // Example C: The same constexpr function running at RUNTIME.
    // -----------------------------------------------------------------
    int runtime_base = 2;
    int runtime_exp = 8; // Imagine this was read from user input (std::cin).
    
    // Because inputs are NOT constexpr, the compiler treats pow_cpp14 as a normal function.
    // It is executed while the program is running.
    int runtime_result = pow_cpp14(runtime_base, runtime_exp);
    std::cout << "Runtime result of 2^8: " << runtime_result << "\n";

    return 0;
}