#include <iostream>
#include <stack>
#include <string>
#include <thread>
#include <vector>

#include "consumer.h"
#include "producer.h"

int main()
{
	std::mutex m_mutex;
	std::condition_variable m_conditionFull;
	std::condition_variable m_conditionEmpty;
	std::stack<std::string> m_stack;
	std::string m_path;
	const int producerCount = 2;
	std::vector<std::thread>producerThreads(producerCount);
	std::vector<char> m_finished(producerCount, 0);


	for (int i = 0; i < producerCount; i++)
	{
		producerThreads[i] = std::thread(Producer(
			"text.txt"
			, std::ref(m_stack)
			, std::ref(m_mutex)
			, std::ref(m_conditionFull)
			, std::ref(m_conditionEmpty)
			, std::ref(m_finished[i]))
		);
	}

	std::thread threadC(Consumer(std::ref(m_stack)
		, std::ref(m_mutex)
		, std::ref(m_conditionFull)
		, std::ref(m_conditionEmpty)
		, std::ref(m_finished))
	);

	for (auto& thread : producerThreads)
	{
		thread.join();
	}

	threadC.join();
}