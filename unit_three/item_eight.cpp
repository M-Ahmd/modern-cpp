#include<iostream>

using namespace std;
void fun(int);
void fun(bool);
void fun(void *);


int * fun(){
    return nullptr;
}

int main(){
    fun(0);
    //fun(NULL);will break
    fun(nullptr);//the correct 

    //more readable for programmer 
    auto f = fun();
    // if(f == 0)//bad use the programmer that not see the fun will excpect f return int 
    // {
        
    // }
    if(f == nullptr);//good use 
}

void fun(int i){
    cout << "i am a integer";
}
void fun(bool i){
    cout << "i am a bool";
}
void fun(void * ptr){
    cout << "i am a ptr";
}