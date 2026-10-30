#include<iostream>
using namespace std;
std::unique_ptr<int> factory_function() {
	return make_unique<int>(100);
}
int main() {
	unique_ptr<int> p1 = make_unique<int>(100);

	unique_ptr<int> p2 = std::move(p1);

	unique_ptr<int> p3 = factory_function();//i just the return is r value 
											//we use move if we want to convert
											//l value to r value

	auto closeFile = [](FILE* f) {
		fclose(f);
	};
	//the myfile ptr is the same as raw ptr if the lamda remained [] 
	unique_ptr<FILE, decltype(closeFile)>myfile(fopen("data.txt", "w"), closeFile);

	//we can easly convert unique_ptr to shared_ptr so easy

	shared_ptr<int> shared = factory_function();
}
