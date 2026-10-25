/*
 * Item 10: Prefer scoped enums (enum class) to unscoped enums (enum)
 * Summary: Always use enum class because it is safer, prevents namespace pollution, and supports forward declaration.
 */

#include <iostream>
#include <tuple>
#include <string>

// 1. Forward Declaration
enum class Status; // (Valid) The compiler knows the default underlying type is int.
// enum Color;     // (Invalid) The compiler doesn't know how much memory to allocate!

// 2. Namespace Pollution
enum OldColor { White, Black }; 
// bool White = true; // Error! The name 'White' is already reserved by the unscoped enum.

enum class NewColor { White, Black };
bool White = true; // (Valid) The enum's 'White' is scoped and must be accessed via NewColor::White.


int main() {
    // 3. Type Safety (Preventing implicit conversions)
    OldColor old_c = Black;
    if (old_c < 10) { 
        // The compiler allows this disaster and implicitly converts the color to an integer!
    }

    NewColor new_c = NewColor::Black;
    // if (new_c < 10) // The compiler strongly rejects this, protecting you from logical errors.
    
    if (static_cast<int>(new_c) < 10) {
        // If you intentionally want a number, you must cast it explicitly to take responsibility.
    }

    // =================================================================
    // 4. The Only Exception (When to use the unscoped enum?)
    // When reading data from std::tuple, the unscoped enum saves a lot of typing.
    // =================================================================
    using UserInfo = std::tuple<std::string, std::string, int>;
    UserInfo uInfo{"Ahmed", "ahmed@mail.com", 22};

    // Using the unscoped enum (Implicit conversion to integer makes the code very readable)
    enum OldFields { Name, Email, Age }; 
    std::cout << std::get<Email>(uInfo) << "\n"; 

    // Using the scoped enum (Requires an annoying explicit cast every time)
    enum class NewFields { Name, Email, Age };
    std::cout << std::get<static_cast<std::size_t>(NewFields::Email)>(uInfo) << "\n";

    return 0;
}