#include<iostream>
#include<vector>
using namespace std;

//instead of using this
template<typename T>
struct some_thing{
    typedef vector<T> mytype;
};

//do this 
template<typename T>
using modern_type = vector<T>;

int main(){
    some_thing<int>::mytype t1;
    modern_type<int> t2;
}