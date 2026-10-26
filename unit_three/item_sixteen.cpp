#include <iostream>
#include <mutex>
#include <atomic>
#include <vector>
#include <thread>

// =====================================================================
// Item 16: Make const member functions thread safe
// =====================================================================
// Why? Because clients expect that calling a 'const' function from 
// multiple threads simultaneously is 100% safe (no data races).
// If you use 'mutable' inside a const function, YOU must provide the synchronization.

class ThreadSafeMath {
public:
    // -----------------------------------------------------------------
    // CASE 1: Using std::mutex for complex mutable data (e.g., Caching)
    // -----------------------------------------------------------------
    int getExpensiveValue() const {
        // 1. Lock the mutex before reading or writing to the mutable variables.
        // std::lock_guard automatically unlocks when the function finishes.
        std::lock_guard<std::mutex> guard(m_mutex);
        
        // 2. Safely check and update the cache
        if (!m_cacheValid) {
            // Simulate a heavy calculation here...
            m_cachedValue = 42; 
            m_cacheValid = true;
        }
        
        return m_cachedValue;
    } // Mutex is automatically unlocked here

    // -----------------------------------------------------------------
    // CASE 2: Using std::atomic for simple mutable variables (e.g., Counters)
    // -----------------------------------------------------------------
    int getCallCount() const noexcept {
        // std::atomic guarantees that this increment is thread-safe.
        // No mutex is needed, making it much faster for simple types.
        ++m_callCount; 
        
        return m_callCount;
    }

private:
    // --- Data for CASE 1 ---
    // Note: The mutex itself MUST be mutable so we can lock it inside a const function!
    mutable std::mutex m_mutex; 
    mutable bool m_cacheValid{false};
    mutable int m_cachedValue{0};

    // --- Data for CASE 2 ---
    mutable std::atomic<int> m_callCount{0}; 
};

// =====================================================================
// Simulation: Calling the const functions from multiple threads
// =====================================================================
void run_simulation(const ThreadSafeMath& mathObj) {
    // Both of these calls are completely thread-safe now!
    mathObj.getExpensiveValue();
    mathObj.getCallCount();
}

int main() {
    ThreadSafeMath mathObj;

    // Create two threads that access the SAME object at the SAME time
    std::thread t1(run_simulation, std::ref(mathObj));
    std::thread t2(run_simulation, std::ref(mathObj));

    t1.join();
    t2.join();

    std::cout << "Total calls made securely: " << mathObj.getCallCount() << "\n";

    return 0;
}