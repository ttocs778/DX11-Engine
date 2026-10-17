#pragma once
#include <atomic>
#include <mutex>
#include <queue>
#include <vector>
#include "Job.h"
#include "Worker.h"

class JobManager {
public:
	JobManager();
	~JobManager();

	// Start all workers.
	void Begin();
	// Set the shutdown flag and end workers.
	void End();

	// Add a job to the FIFO queue.
	void AddJob(Job* pJob);

	// Wait until all added jobs have been processed.
	void WaitForJobs();

	// Accessors for use by Worker::Loop.
	bool IsShutdown() const { return mShutdown.load(); }
	std::mutex& GetQueueMutex() { return mQueueMutex; }
	std::queue<Job*>& GetJobQueue() { return mJobQueue; }
	void DecrementJobCount() { mJobCount--; }

	// Singleton accessor.
	static JobManager* Get();

	std::atomic<bool> mShutdown;
	std::atomic<int> mJobCount;
	std::mutex mQueueMutex;
	std::queue<Job*> mJobQueue;
	std::vector<Worker*> mWorkers;
	static JobManager* s_pJobManager;
};
