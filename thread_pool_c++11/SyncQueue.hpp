#include<list>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<iostream>
using namespace std;

template<typename T>
class SyncQueue
{
public:
	SyncQueue(int maxSize) :m_maxSize(maxSize), m_needStop(false)
	{

	}

	void Put(const T&x)
	{
		Add(x);
	}
	//forwardת��
	void Put(T&&x)
	{
		Add(std::forward<T>(x));
	}

	//ͨ��move�﷨����ȡ�������У��������
	void Take(std::list<T> &list)
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty(); });
		if (m_needStop)
			return;
		list = std::move(m_queue);
		m_notFull.notify_one();
	}
	void Take(T& t)
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty(); });

		if (m_needStop)
			return;
		t = m_queue.front();
		m_queue.pop_front();
		//��һ��λ�ÿ��У�nofity_one֪ͨһ��Ŀ�꣬Ŀ���ǲ�ȷ����
		m_notFull.notify_one();
	}

	void Stop()
	{
		{
			std::lock_guard<std::mutex> locker(m_mutex);
			m_needStop = true;
		}
		//֪ͨ���еȴ��е��ź�
		m_notEmpty.notify_all();
		m_notFull.notify_all();
	}

	bool Empty()
	{
		std::lock<std::mutex> locker(m_mutex);
		return m_queue.empty();
	}

	bool Full()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		retrun m_queue.size() == m_maxSize;
	}

	size_t Size()
	{
		return m_queue.size();
	}
private:
	template<typename F>
	void Add(F &&x)
	{
		std::unique_lock< std::mutex> locker(m_mutex);
		m_notFull.wait(locker, [this]{return m_needStop || NotFull(); });
		if (m_needStop)
			return;
		m_queue.push_back(std::forward<F>(x));
		m_notEmpty.notify_one();
	}

	bool NotFull() const
	{
		bool full = m_queue.size() >= m_maxSize;
		if (full)
			cout << "full, waiting��thread id: " << this_thread::get_id() << endl;
		return !full;
	}
	bool NotEmpty() const
	{
		bool empty = m_queue.empty();
		if (empty)
			cout << "empty,waiting��thread id: " << this_thread::get_id() << endl;
		return !empty;
	}
private:
	std::list<T> m_queue; //������
	std::mutex m_mutex; //�����������������������ʹ��
	std::condition_variable m_notEmpty;//��Ϊ�յ���������
	std::condition_variable m_notFull; //û��������������
	int m_maxSize; //ͬ����������size

	bool m_needStop; //ֹͣ�ı�־
};