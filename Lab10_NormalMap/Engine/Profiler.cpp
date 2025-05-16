#include "stdafx.h"
#include "Profiler.h"
#include <cassert>
#include <algorithm>
#include <functional>

void Profiler::Timer::Start() {
	mStartTime = std::chrono::high_resolution_clock::now();

	uint64_t ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
		mStartTime.time_since_epoch()).count() / 1000;
	Profiler::Get()->BeginTimer(mName, ts);
}

void Profiler::Timer::Stop() {
	auto endTime = std::chrono::high_resolution_clock::now();
	double elapsed = std::chrono::duration<double, std::milli>(endTime - mStartTime).count();
	mCurrentFrameDuration += elapsed;

	// Calculate timestamp in microseconds:
	uint64_t ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
		endTime.time_since_epoch()).count() / 1000;
	// Write JSON end event.
	Profiler::Get()->EndTimer(ts);
}

void Profiler::Timer::Reset() {
	mTotalTime += mCurrentFrameDuration;
	mMaxFrameDuration = Math::Max(mMaxFrameDuration, mCurrentFrameDuration);
	++mFrameCount;
	mCurrentFrameDuration = 0.0;
}


Profiler::Profiler() {
	errno_t err = fopen_s(&m_pProfileFile, "profile.json", "w");
	assert(err == 0 && m_pProfileFile);
	fprintf(m_pProfileFile, "[\n");
	m_bFirstEvent = true;
}

Profiler::~Profiler() {

	fprintf(m_pProfileFile, "\n]\n");
	fclose(m_pProfileFile);

	FILE* pFile = nullptr;
	errno_t err = fopen_s(&pFile, "profile.txt", "w");
	if (err != 0 || pFile == nullptr) {
		assert(false && "Failed to open profile.txt for writing");
	}
	else {
		fprintf(pFile, "Name,\tAverage (ms),\tMax (ms)\n");
		for (auto& pair : mTimers) {
			Profiler::Timer* timer = pair.second;
			fprintf(pFile, "%s,\t%lf,\t%lf\n",
				timer->GetName().c_str(),
				timer->GetAvg_ms(),
				timer->GetMax_ms());
		}
		fclose(pFile);
	}


	for (auto& pair : mTimers) {
		delete pair.second;
	}
	mTimers.clear();
}

Profiler::Timer* Profiler::GetTimer(const std::string& name) {
	auto it = mTimers.find(name);
	if (it != mTimers.end()) {
		return it->second;
	}
	else {
		Timer* newTimer = new Timer(name);
		mTimers[name] = newTimer;
		return newTimer;
	}
}

void Profiler::ResetAll() {
	for (auto& entry : mTimers) {
		entry.second->Reset();
	}
}

void Profiler::BeginTimer(const std::string& name, uint64_t startTime) {
	if (!m_bFirstEvent) {
		fprintf(m_pProfileFile, ",\n");
	}
	else {
		m_bFirstEvent = false;
	}

	uint64_t tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
	fprintf(m_pProfileFile, "{\"name\":\"%s\",\"cat\":\"PERF\",\"ph\":\"B\",\"ts\":%llu,\"pid\":1,\"tid\":%llu}",
		name.c_str(), startTime, tid);
}

void Profiler::EndTimer(uint64_t endTime) {
	fprintf(m_pProfileFile, ",\n");
	uint64_t tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
	fprintf(m_pProfileFile, "{\"ph\":\"E\",\"ts\":%llu,\"pid\":1,\"tid\":%llu}",
		endTime, tid);
}
