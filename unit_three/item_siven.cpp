#include <iostream>
#include <initializer_list> 

using namespace std;

class test_init {
public:
    void print() {
        cout << "hello world\n";
    } 
};

class program {
    test_init *t1{};

public:
    program() {
        t1 = new test_init();
    }
    ~program() {
        delete t1;
    }
    void fun() {
        t1->print();
    }
};

template<typename T>
class vector {
    T *arr; 

public:
    vector(std::initializer_list<T> list) {
        arr = new T[list.size()]; 
        
        int i = 0;
        for(auto it = list.begin(); it != list.end(); ++it) {
            arr[i++] = *it;
        }
    }
    
    ~vector() {
        delete[] arr;
    }
}; 

int main() {
    double x = 1.2;
    int y(x);
    cout << "y = " << y << "\n";
    
    
    program g1;
    g1.fun();
    
    vector<int> myVec{10, 20, 30}; // will call the init list constractor
    
    return 0;
}