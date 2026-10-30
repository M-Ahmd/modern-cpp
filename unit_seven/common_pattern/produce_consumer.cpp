#include<iostream>
#include<thread>
#include<mutex>
#include<memory>
#include<chrono>
#include<condition_variable>
#define VALUE 10

using namespace std;
template<typename T>
struct node{
	T data;
	unique_ptr<node<T>> next;
};
template<typename T>
class SafeQueue{
public:

	void push(T data){
		unique_ptr<node<T>> tem = make_unique<node<T>>();
		tem->data = data;
		{
			lock_guard<mutex> lock(mtx);
			tem->next = move(head);
			head = move(tem);
		}
		cv.notify_one();
	}

	T pop(){
		unique_lock<mutex> lock(mtx);

		cv.wait(lock, [&]{return head != nullptr;});
		
		T data = head->data;

		head = move(head->next);

		return data;
	}



private:
	unique_ptr<node<T>> head{};
	mutex mtx;
	condition_variable cv;
};
void produce(SafeQueue<int> &q){
	while(true){
		q.push(VALUE);
		cout << "a new value is produced" << endl;
		this_thread::sleep_for(chrono::milliseconds(500));
	}
}
void consume(SafeQueue<int> &q){
	while(true){
		q.pop();
		cout << "a new value is consumed" << endl;
		this_thread::sleep_for(chrono::milliseconds(500));
	}
}
int main(){
	SafeQueue<int> q;
	thread consumer(consume, ref(q));
	thread producer(produce, ref(q));
	
	// remember my friend that any part has a global variable is a critial path
	consumer.join();
	producer.join();
}
