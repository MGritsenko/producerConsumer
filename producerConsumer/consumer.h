#pragma once

#include <string>
#include <mutex>
#include <condition_variable>
#include <stack>

class Consumer
{
public:
	Consumer(std::string&& pattern
		, std::stack<std::string>& stack
		, std::mutex& mutex
		, std::condition_variable& conditionFull
		, std::condition_variable& conditionEmpty
		, bool& finished);

	void operator()();

	void processData();
	void hasPattern() const;

private:
	std::mutex& m_mutex;
	std::condition_variable& m_conditionFull;
	std::condition_variable& m_conditionEmpty;
	std::stack<std::string>& m_stack;
	const std::string m_pattern;
	bool& m_finished;

	bool m_hasPattern = false;
};

