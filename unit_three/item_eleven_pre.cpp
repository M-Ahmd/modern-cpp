#include<iostream>
using namespace std;

class Car{
    int len;
    int height;
    int wieght;

public:
    Car(int len, int height, int wieght) : len(len),height(height),wieght(wieght){

    }
    Car(const Car &copy){//copy constactor
        len = copy.len ;
        height = copy.height;
        wieght = copy.wieght;
    }
    Car(){

    }
    Car& operator=(const Car &copy){//assignment copy
        if(this != &copy){
            len = copy.len;
            height = copy.height;
            wieght = copy.wieght;
        }
        return *this;
    }

    void print(){
        cout << "the len is " << len << " the height is " << height << " the weight is " << wieght << endl;
    }

};


int main(){
    Car car1(10, 20, 30);

    Car car2 = car1;
    car2.print();

    Car car3(10, 20, 30);
    Car car4;

    car4 = car3;

    car4.print();
}