#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

int main(){
	vector<int> vect{10, 40, 31, 2};
	//in some stl we inject the logic into a lamda express that return bool
	sort(vect.begin(), vect.end(), [](int a, int b){return a > b;});

	for(auto it = vect.begin(); it != vect.end(); it++){
		cout << *it << " ";
	}

	int hello = 10;

	auto fun = [&](){
		hello++;
	};

	cout << "the value of the hello is " << hello << endl;

	fun();

	cout << "the value of the hello is " << hello << endl;

	return 0;
}
