#pragma once

#include <string>
#include <mutex>
#include <condition_variable>
#include <stack>

class Producer
{
public:
	Producer(std::string&& path
		, std::stack<std::string>& stack
		, std::mutex& mutex
		, std::condition_variable& conditionFull
		, std::condition_variable& conditionEmpty
		, bool& finished);

	void operator()();

	void prepareData();

private:
	std::mutex& m_mutex;
	std::condition_variable& m_conditionFull;
	std::condition_variable& m_conditionEmpty;
	std::stack<std::string>& m_stack;
	const std::string m_path;
	bool& m_finished;
};

