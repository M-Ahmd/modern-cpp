#include<iostream>

using namespace std;
/*case 1:
ParamType is a Reference or Pointer, but not a Universal
Reference
*/
template<typename T>
void print(T&& param){
    cout << param << endl; /*
    will resolve the T as int and the paramType as int &*/
}
/*case 2 : 
ParamType is a Universal Reference
*/
template<typename T>
void print2(T&& param){
    cout << param << endl; /*
    will resolve the T as int and the paramType as int &
    if lvalue else it will be int && (rvalue)
    */
}

/*case 3 : 
ParamType is a not case 1 and case 2
*/
template<typename T>
void print3(T param){
    param = 27;
    cout << param << endl; /*
    will copy just copy the value
    */
}


int main(){
    int x = 10;
    print(x);
    print2(x);
    print2(27);
    print3(x);
    print(x);//will remain 10 as it is 
}