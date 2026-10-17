#include "stdafx.h"
#include "stdafx.h"
#include "Worker.h"
#include "JobManager.h"
#include <chrono>
#include <thread>

std::atomic<bool> Worker::sShutdown{ false };

Worker::Worker()
{
}

Worker::~Worker()
{
	End();
}
void Worker::Begin()
{
	mThread = std::thread(Worker::Loop);
}

void Worker::End()
{
	sShutdown.store(true);
	if (mThread.joinable()) {
		mThread.join();
	}
}

void Worker::Loop()
{
	while (!sShutdown.load()) {
		Job* pJob = nullptr;
		{
			std::lock_guard<std::mutex> lock(JobManager::Get()->mQueueMutex);
			if (!JobManager::Get()->mJobQueue.empty()) {
				pJob = JobManager::Get()->mJobQueue.front();
				JobManager::Get()->mJobQueue.pop();
			}
		}


		if (pJob) {
			pJob->DoIt();
			JobManager::Get()->mJobCount--;
			// Cleanup the job.
			delete pJob;
		}
		else {
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
	}
}

void Worker::SetShutdown(bool flag)
{
	sShutdown.store(flag);
}
