#include<iostream>
using namespace std;
template<typename T>
class TypeDisplayer;



int main(){

    auto i = 10;

    TypeDisplayer<decltype(i)> obj;

}