#include<iostream>
using namespace std;
int main() {
	shared_ptr<int> p1 = make_shared<int>(10);

	shared_ptr<int> p2 = p1;


	auto clean = [](FILE* file) {
		fclose(file);
	};

	shared_ptr<FILE> myfile(fopen("data.txt", "w"), clean);
	//note there that we don't tell it in the <> the type
	//this will do some intearsing thing will take the code 
	// of the clean function and put it in the control block of it
	// the shared_ptr has two thing 1) ptr for object
	// 2) and ptr_for heap control block

}
