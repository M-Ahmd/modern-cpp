#include<iostream>
#include<future>
#include<vector>
#include<chrono>
#include<thread>
using namespace std;
int sum_part(int start, int end, vector<int> &array){
    this_thread::sleep_for(chrono::seconds(1));
    int res = 0;
    for(int i = start; i < end; i++){
        res += array[i];
    }
    return res;
}

int main(){
    vector<int> array(150, 1);
    auto r1 = async(launch::async, sum_part, 0, 50, ref(array));
    auto r2 = async(launch::async, sum_part, 50, 100, ref(array));
    auto r3 = async(launch::async, sum_part, 100, 150, ref(array));

    int res1 = r1.get();
    int res2 = r2.get();
    int res3 = r3.get();
    cout << res1 << " " << res2 << " " << res3 << endl;

    auto res = res1 + res2 + res3;
    cout << res << endl;
    return 0;
}