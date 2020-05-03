#include "producer.h"

#include <iostream>
#include <assert.h>
#include <fstream>

Producer::Producer(
	  std::string&& path
	, std::stack<std::string>& stack
	, std::mutex& mutex
	, std::condition_variable& conditionFull
	, std::condition_variable& conditionEmpty
	, bool& finished)

    : m_stack(stack)
    , m_mutex(mutex)
    , m_conditionFull(conditionFull)
    , m_conditionEmpty(conditionEmpty)
    , m_path(path)
    , m_finished(finished)
{

}

void Producer::operator()()
{
    prepareData();
}

void Producer::prepareData()
{
	std::ifstream file(m_path);
    if (!file.is_open())
    {
        assert(!"File is not open");
    }

	std::string line;
	while (std::getline(file, line, '\n'))
	{    
		{
			std::unique_lock<std::mutex> uniqueLock(m_mutex);
			m_conditionEmpty.wait(uniqueLock, [&]
			{
				return m_stack.empty();
			});

			m_stack.push(line);
		}

		m_conditionFull.notify_one();
	}

	/*{
		std::lock_guard<std::mutex> lockGuard(m_mutex);
		m_finished = true;
	}*/

	//m_conditionFull.notify_one();

    file.close();
}
