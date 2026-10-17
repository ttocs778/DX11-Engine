#pragma once

#include <thread>
#include <atomic>
#include <chrono>

class Worker {
public:
	Worker();
	~Worker();

	void Begin();

	void End();

	static void Loop();

	static void SetShutdown(bool flag);

private:
	std::thread mThread;
	static std::atomic<bool> sShutdown;
};



