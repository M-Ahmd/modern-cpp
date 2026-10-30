#include<iostream>
#include<memory>
using namespace std;

//1) use the move in the last step
void ProcessPage(std::unique_ptr<Page>&& page) {


    LogPageId(page->GetPageId());
    ValidateHeader(page->GetData());


    disk_scheduler_.Enqueue(std::move(page)); 


}

//2) don't use move in the return because by defual return return a r value

std::vector<int> MakeBufferBad() {
    std::vector<int> buffer(4096);
    return std::move(buffer);//takes time in the create a local object and make it r value
}


std::vector<int> MakeBufferGood() {
    std::vector<int> buffer(4096);
    return buffer; // the default is r value
}
//when we use move in the return ?
//when the r value is baram in the function and we want to return it
std::unique_ptr<Page> ForwardPage(std::unique_ptr<Page>&& page) {
    return std::move(page);
}
int main(){return 0;}
