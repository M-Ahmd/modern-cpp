#include <iostream>
using namespace std;

int main() {
    // Case 3: Pass-by-value (Template Type Deduction)
    auto x = 27;        // Deduced as: int x = 27;
    cout << "x = " << x << endl;

    // Case 1: Reference 
    auto &y = x;        // Deduced as: int& y = x;
    cout << "y = " << y << endl;

    // Case 2: Universal Reference with lvalue
    auto &&lvalue = x;  // Deduced as: int& lvalue = x;
    cout << "lvalue = " << lvalue << endl;

    // Case 2: Universal Reference with rvalue
    auto &&rvalue = 10; // Deduced as: int&& rvalue = 10;
    cout << "rvalue = " << rvalue << endl;

    // Direct Initialization
    int var(76);
    cout << "var = " << var << endl;

    // Uniform Initialization (C++17 behavior)
    int variable{20};   // Deduced as int, not std::initializer_list
    cout << "variable = " << variable << endl;

    return 0;
}