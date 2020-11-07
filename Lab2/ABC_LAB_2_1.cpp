
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <ctime>
#include <functional>
using namespace std;

class Calculation
{
public:
	int numTasks;
	int numThreads;
	vector<_int8> tasks;

	Calculation(int numTasks, int numThreads) {
		this->numTasks = numTasks;
		this->numThreads = numThreads;
		tasks = vector<_int8>(numTasks, 0);
	}

	virtual bool getCounter(int& localCounter) = 0;

	virtual void threadFunction() 
	{
		while (true)
		{
			int localCounter;
			if (getCounter(localCounter)) {
				break;
			}
			tasks[localCounter] = (_int8)1;
			this_thread::sleep_for(std::chrono::nanoseconds(20));
		}
	}

	void Run() {
		thread* threads = new thread[numThreads];
		for (int i = 0; i < numThreads; i++) {
			threads[i] = thread([this]() { this->threadFunction(); });
		}

		for (int i = 0; i < numThreads; i++) {
			threads[i].join();
		}

		for (int i = 0; i < tasks.size(); i++) {
			if ((int)tasks[i] == 0) {
				cout << "Error";
			}
		}

		delete[] threads;
	}
private:
};


class AtomicCalculation : public Calculation {
	atomic<int> counter;
public:
	AtomicCalculation(int numTasks, int numThreads) : Calculation(numTasks, numThreads)
	{
		this->counter = numTasks - 1;
	}

	bool getCounter(int& localCounter) override {
		localCounter = counter--;
		if (localCounter < 0) {
			return true;
		}
		return false;
	}
};


class MutexCalculation : public Calculation {
	mutex g_lock;
	int counter;
public:
	MutexCalculation(int numTasks, int numThreads) : Calculation(numTasks, numThreads)
	{
		this->counter = numTasks - 1;
	}

	bool getCounter(int& localCounter) {
		g_lock.lock();
		localCounter = counter--;
		g_lock.unlock();
		if (localCounter < 0) {
			return true;
		}
		return false;
	}
};



int main()
{
	MutexCalculation mutexCalculation(4 * 1024 * 1024, 1);
	AtomicCalculation atomicCalculation(4 * 1024 * 1024, 1);
	unsigned int start_time = clock();
	mutexCalculation.Run();
	//atomicCalculation.Run();
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time;
	cout << search_time;
	return 0;
}