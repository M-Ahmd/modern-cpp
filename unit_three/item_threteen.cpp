#include <iostream>
#include <vector>
#include <algorithm>

// =====================================================================
// ITEM 13: Prefer const_iterators to iterators & Use Non-member functions
// =====================================================================

// BEST PRACTICE: Generic Search Function (Works with ANY container)
template <typename Container, typename Target>
void findAndPrint(const Container& c, const Target& val) {
    // 1. Use Non-member functions (std::cbegin, std::cend) for max compatibility.
    // 2. We use 'cbegin' because we only READ data, we don't modify it.
    auto it = std::find(std::cbegin(c), std::cend(c), val);
    
    if (it != std::cend(c)) {
        std::cout << "Found: " << *it << "\n";
        // *it = 100; // ERROR! const_iterator protects your data
    } else {
        std::cout << "Not found\n";
    }
}

int main() {
    // 1. Works beautifully with modern STL containers
    std::vector<int> vec = {1983, 1998, 2011, 2014};
    findAndPrint(vec, 1998);

    // 2. Works beautifully with raw C-Arrays (Thanks to non-member functions)
    int arr[] = {10, 20, 30, 40};
    findAndPrint(arr, 30);
    
    return 0;
}