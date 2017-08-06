#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>
#include <condition_variable>

#define BE_FAIR 1
#define WAITING_THRESHOLD 50

// So we can use shorthands for during "2s" are 2 seconds etc
using namespace std::literals::chrono_literals;

std::atomic<bool> g_shouldStart;
std::atomic<bool> g_shouldQuit;

enum class AnimalType { Lion, Rabbit };

class WaterSource
{
public:
	void enter(AnimalType type)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
#if BE_FAIR
		if ((m_animalsWaiting <= WAITING_THRESHOLD && type == m_type) || m_animalCount == 0)
#else
		if (type == m_type || m_animalCount == 0)
#endif
		{
			m_animalCount++;
			updateBalance(type);
		}
		else
		{
			m_animalsWaiting++;
#if BE_FAIR
			m_cond.wait(lock, [this,type] {
					return ((m_animalsWaiting <= WAITING_THRESHOLD && type == m_type) || m_animalCount == 0);});
#else
			m_cond.wait(lock, [this,type]{
					return (m_type == type || m_animalCount==0);});
#endif
			--m_animalsWaiting;
			m_animalCount++;
			updateBalance(type);
		}
	}

	// Pre: type==m_type
	void leave(AnimalType type)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_animalCount--;
		m_cond.notify_all();
	}

	int getBalance(AnimalType type)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		switch (type)
		{
			case AnimalType::Lion:
				return m_lionBalance;
			case AnimalType::Rabbit:
				return m_rabbitBalance;
		}
	}
private:
	// Pre: Mutex must be locked!
	void updateBalance(AnimalType type)
	{
		switch (type)
		{
			case AnimalType::Lion:
				++m_lionBalance;
				break;
			case AnimalType::Rabbit:
				++m_rabbitBalance;
				break;
		}
	}

	int m_animalsWaiting = 0;
	int m_animalCount = 0;
	AnimalType m_type = AnimalType::Lion;

	std::mutex m_mutex;
	std::condition_variable m_cond;

	int m_lionBalance = 0;
	int m_rabbitBalance = 0;
} g_waterSource;

void lifecycle_lion(void)
{
	while (!g_shouldStart){} // Make sure all animals start at the same time
	while (!g_shouldQuit)
	{
		std::this_thread::sleep_for(2ms);
		g_waterSource.enter(AnimalType::Lion);
		std::this_thread::sleep_for(20ms);
		g_waterSource.leave(AnimalType::Lion);
	}
}

void lifecycle_rabbit(void)
{
	while (!g_shouldStart){} // Make sure all animals start at the same time
	while (!g_shouldQuit)
	{
		std::this_thread::sleep_for(2ms);
		g_waterSource.enter(AnimalType::Rabbit);
		std::this_thread::sleep_for(20ms);
		g_waterSource.leave(AnimalType::Rabbit);
	}
}

int main(int argc, char* argv[])
{
	g_shouldStart = false;
	g_shouldQuit = false;

	size_t num_lion_threads = 100;
	size_t num_rabbit_threads = 100;
	std::vector<std::thread> m_threads;

	for (size_t i=0; i<num_lion_threads; ++i)
		m_threads.emplace_back(lifecycle_lion);
	for (size_t i=0; i<num_rabbit_threads; ++i)
		m_threads.emplace_back(lifecycle_rabbit);

	g_shouldStart = true;
	std::this_thread::sleep_for(2s);

	// Signal that we'd like to wrap this thing up
	g_shouldQuit = true;
	for (auto& t : m_threads)
		t.join();

	int balance_lions = g_waterSource.getBalance(AnimalType::Lion);
	int balance_rabbits = g_waterSource.getBalance(AnimalType::Rabbit);
	std::cout << 
		"Balance: \n"
		"\tLions: " << balance_lions << "\n"
		"\tRabbits: " << balance_rabbits << "\n"
		"\tTotal: " << balance_rabbits+balance_lions << "\n";

	return 0;
}

