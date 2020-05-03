#include <iostream>
#include <stack>
#include <string>
#include <thread>

#include "consumer.h"
#include "producer.h"

int main()
{
	std::mutex m_mutex;
	std::condition_variable m_conditionFull;
	std::condition_variable m_conditionEmpty;
	std::stack<std::string> m_stack;
	std::string m_path;
	bool m_finished = false;

	//Producer producer(std::move("text.txt"), m_stack, m_mutex, m_conditionVariable, m_finished);
	//Consumer consumer(std::move("BUBBABBA"), m_stack, m_mutex, m_conditionVariable, m_finished);
	//std::thread threadP(producer);
	//std::thread threadC(consumer);

	std::thread threadP(Producer(
		std::move("text.txt")
		, std::ref(m_stack)
		, std::ref(m_mutex)
		, std::ref(m_conditionFull)
		, std::ref(m_conditionEmpty)
		, std::ref(m_finished))
	);
	std::thread threadC(Consumer(
		std::move("BUBBABBA")
		, std::ref(m_stack)
		, std::ref(m_mutex)
		, std::ref(m_conditionFull)
		, std::ref(m_conditionEmpty)
		, std::ref(m_finished))
	);

	threadP.join();
	threadC.join();
}