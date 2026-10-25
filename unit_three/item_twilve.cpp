#include <iostream>
#include <vector>
#include <utility> // For std::move

using namespace std;

// =====================================================================
// ITEM 12: Declare overriding functions override & Reference Qualifiers
// =====================================================================

class base {
public:
    // BEST PRACTICE: Always add a virtual destructor in polymorphic base classes
    virtual ~base() = default;

    virtual void fun() & {
        cout << "i am the father lvalue\n";
    }
    
    virtual void fun() && {
        cout << "i am the father rvalue\n";
    }
};

class driven : public base {
public:
    // Using 'override' ensures the compiler checks for exact signature matches
    void fun() & override {
        cout << "i am the driven lvalue\n";
    }
    
    void fun() && override {
        cout << "i am the driven rvalue\n";
    }
};

class Widget {
public:
    using DataType = std::vector<double>;

    // 1. For lvalue Widgets: Safely return an lvalue reference
    DataType& data() & { 
        return values; 
    } 

    // 2. For rvalue Widgets: Steal the vector using std::move to avoid deep copying
    DataType data() && { 
        return std::move(values); 
    } 

private:
    DataType values;
};

int main() {
    base *ptr = new driven();
    
    // 1. Normal call on an lvalue
    ptr->fun(); // Prints: i am the driven lvalue
    
    // 2. Cast the OBJECT (*ptr) to rvalue, not the pointer itself
    std::move(*ptr).fun(); // Prints: i am the driven rvalue
    
    delete ptr; // Clean up memory (Safe because of virtual ~base)
    return 0;
}