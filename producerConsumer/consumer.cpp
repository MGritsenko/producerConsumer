#include "consumer.h"

#include <iostream>
#include <assert.h>

Consumer::Consumer(
	const std::string&& pattern
	, std::stack<std::string>& stack
	, std::mutex& mutex
	, std::condition_variable& conditionFull
	, std::condition_variable& conditionEmpty
	, bool& finished)

	: m_stack(stack)
	, m_mutex(mutex)
	, m_conditionFull(conditionFull)
	, m_conditionEmpty(conditionEmpty)
	, m_pattern(pattern)
	, m_finished(finished)
{
}

void Consumer::operator()()
{
	processData();
	hasPattern();
}

void Consumer::hasPattern() const
{
	if (m_hasPattern)
	{
		std::cout << "HAS PATTERN!";
	}
}

void Consumer::processData()
{
	int counter = 0;

	while (true)
	{
		std::unique_lock<std::mutex> uniqueLock(m_mutex);

		m_conditionFull.wait(uniqueLock, [&]
		{
			return !m_stack.empty() || m_finished;
		});

		std::stack<std::string> stack = std::move(m_stack);
		uniqueLock.unlock();

		m_conditionEmpty.notify_one();

		while (!stack.empty())
		{
			const auto pattern = stack.top();
			stack.pop();

			counter += std::stoi(pattern);

			//std::cout << counter << std::endl;

			if (counter == 200000)
			{
				break;
			}
		}

		uniqueLock.lock();
		if (m_finished)
		{
			break;
		}
	}

	assert(counter == 200000);
}
