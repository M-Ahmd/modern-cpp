#include<iostream>
#include <memory>
using namespace std;

int main(){

	auto ptr = make_unique<char[]>(10);

	auto move_ptr = std::move(ptr);//we cannot copy it take care if the ptr is const will copy it

	return 0;

}
