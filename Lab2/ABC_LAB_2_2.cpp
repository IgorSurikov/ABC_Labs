
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <functional>
using namespace std;


class MutexQueue
{
	mutex g_lock;
	queue<uint8_t> elements;
public:
	void push(uint8_t val) {
		g_lock.lock();
		elements.push(val);
		g_lock.unlock();
	}

	bool pop(uint8_t& val) {
		g_lock.lock();
		if (elements.size() > 0) {
			val = elements.front();
			elements.pop();
			g_lock.unlock();
			return true;
		}
		g_lock.unlock();
		return false;
	}

	int len() {
		return elements.size();
	}
};





class ConsumerFactory
{
public:
	ConsumerFactory(MutexQueue* q, int consumerNumber) {
		this->q = q;
		this->consumerNumber = consumerNumber;
		sums = vector<int>(consumerNumber, 0);
	}

	void consumerFunction(int i)
	{
		while (1) {
			uint8_t val = 0;
			if (q->pop(val)) {
				g_lock.lock();
				sums[i] += val;
				g_lock.unlock();
				continue;
			}
			this_thread::sleep_for(std::chrono::milliseconds(2));
			if (q->pop(val)) {
				g_lock.lock();
				sums[i] += val;
				g_lock.unlock();
				continue;
			}
			else
			{
				break;
			}
		}
	}

	void Run() {
		function<void(int)> getThreadConsumer = bind(&ConsumerFactory::consumerFunction, this, placeholders::_1);
		threads = new thread[consumerNumber];

		for (int i = 0; i < consumerNumber; i++) {
			threads[i] = thread(getThreadConsumer, i);
		}
	}

	void End() {
		for (int i = 0; i < consumerNumber; i++) {
			threads[i].join();
		}

		int sum = 0;
		for (int i = 0; i < sums.size(); i++) {
			cout << sums[i] << '\n';
			sum += sums[i];
		}
		cout << sum;

		delete[] threads;
	}



private:
	MutexQueue* q;
	int consumerNumber;
	vector<int> sums;
	mutex g_lock;
	thread* threads;
};



class ProducerFactory
{
public:
	ProducerFactory(MutexQueue* q,int numTasks, int producerNum) {
		this->numTasks = numTasks;
		this->q = q;
		this->producerNum = producerNum;
	}

	void producerFunction()
	{
		for (int i = 0; i < numTasks; i++)
		{
			q->push(uint8_t(1));
		}
	}

	void Run() {
		function<void()> getThreadProducer = bind(&ProducerFactory::producerFunction, this);
		threads = new thread[producerNum];

		for (int i = 0; i < producerNum; i++) {
			threads[i] = thread(getThreadProducer);
		}
	}

	void End() {
		for (int i = 0; i < producerNum; i++) {
			threads[i].join();
		}

		delete[] threads;
	}

private:
	thread* threads;
	MutexQueue* q;
	int numTasks;
	int producerNum;
};




int main()
{
	int consumerNum = 4;
	int producerNum = 4;
	int numTasks = 1024*4;
	MutexQueue q;
	MutexQueue* qptr = &q;
	ConsumerFactory consumerFactory(qptr, consumerNum);
	ProducerFactory producerFactory(qptr, producerNum, numTasks);

	producerFactory.Run();
	consumerFactory.Run();
	producerFactory.End();
	consumerFactory.End();


	cout << '\n' << "Correct answer "<< producerNum*numTasks;





	return 0;

}


