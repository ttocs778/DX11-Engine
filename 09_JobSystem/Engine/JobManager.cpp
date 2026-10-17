#include "stdafx.h"
#include "JobManager.h"
#include <thread>
#include <chrono>
#include <cassert>
#include "Profiler.h"

JobManager* JobManager::s_pJobManager = nullptr;

JobManager::JobManager()
	: mShutdown(false), mJobCount(0)
{
	// Create 4 worker threads.
	const int numWorkers = 4;
	for (int i = 0; i < numWorkers; ++i) {
		Worker* worker = new Worker();
		mWorkers.push_back(worker);
	}
}

JobManager::~JobManager()
{
	End();

	for (Worker* worker : mWorkers) {
		delete worker;

	}
	mWorkers.clear();

	// Delete any remaining jobs.
	while (!mJobQueue.empty()) {
		delete mJobQueue.front();
		mJobQueue.pop();
	}
}

JobManager* JobManager::Get()
{
	if (!s_pJobManager)
		s_pJobManager = new JobManager();
	return s_pJobManager;
}

void JobManager::Begin()
{
	// Start each worker.
	for (Worker* worker : mWorkers) {
		worker->Begin();
	}
}

void JobManager::End()
{
	// Signal shutdown.
	mShutdown.store(true);
	// End each worker.
	for (Worker* worker : mWorkers) {
		worker->End();
	}
}

void JobManager::AddJob(Job* pJob)
{
	{
		std::lock_guard<std::mutex> lock(mQueueMutex);
		mJobQueue.push(pJob);
	}
	mJobCount++;
}

void JobManager::WaitForJobs()
{
	PROFILE_SCOPE(WaitForJobs);
	while (mJobCount.load() > 0)
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}
