#include<iostream>
#include<vector>
using namespace std;

int arr[] = {1, 2, 3};
int& getelement(int indx){
    return arr[indx];
}

template<typename Container, typename Indx>
decltype(auto) getelement(Container & container, Indx indx){
    return container[indx];
}

int main(){
    /*item three: decltype the problem in the auto that
    follow the template deducing rules if we want to 
    take snapeshot of type
    */
    const int cx = 10;

    auto x = cx;//int &x not const int &
    x = 20;
    cout << x << endl;
    //if we use decltype will take snapeshot
    decltype(cx) var = cx;
    cout << var << endl;

    auto element = getelement(0);
    cout << "hello " << element<< endl;

    decltype(auto) ref = getelement(0);
    ref = 33;
    cout << "the refrence is " << ref << endl;

    vector<int> arr = {1, 2, 3};

    auto &element1 = getelement(arr, 1);
    element1 = 4;
    cout << getelement(arr, 1);
}