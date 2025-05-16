#pragma once
#include "stdafx.h"
#include <unordered_map>
#include <string>
#include <chrono>
#include <cstdio>
#include <thread>
#define PROFILE_SCOPE(name) \
    Profiler::ScopedTimer name##_scope(Profiler::Get()->GetTimer(std::string(#name)))

class Profiler
{
public:
	// Timer inner class.
	class Timer {
		friend class Profiler;
	private:
		Timer(const std::string& name)
			: mName(name)
			, mCurrentFrameDuration(0.0)
			, mMaxFrameDuration(0.0)
			, mTotalTime(0.0)
			, mFrameCount(0)
		{
		}
		~Timer() {}

		std::string mName;
		double mCurrentFrameDuration;
		double mMaxFrameDuration;
		double mTotalTime;
		int mFrameCount;
		std::chrono::high_resolution_clock::time_point mStartTime;

	public:
		void Start();
		void Stop();
		void Reset();

		const std::string& GetName() const { return mName; }
		double GetTime_ms() const { return mCurrentFrameDuration; }
		double GetMax_ms() const { return mMaxFrameDuration; }
		double GetAvg_ms() const { return mFrameCount > 0 ? mTotalTime / mFrameCount : 0.0; }
	};

	class ScopedTimer {
	public:
		ScopedTimer(Timer* timer) : mTimer(timer) {
			if (mTimer)
				mTimer->Start();
		}
		~ScopedTimer() {
			if (mTimer)
				mTimer->Stop();
		}
	private:
		Timer* mTimer;
	};

	static Profiler* Get() {
		static Profiler instance;
		return &instance;
	}

	Timer* GetTimer(const std::string& name);

	void ResetAll();

	void BeginTimer(const std::string& name, uint64_t startTime);

	void EndTimer(uint64_t endTime);

private:
	Profiler();
	~Profiler();

	Profiler(const Profiler&) = delete;
	Profiler& operator=(const Profiler&) = delete;
	std::unordered_map<std::string, Timer*> mTimers;


	FILE* m_pProfileFile = nullptr;
	bool m_bFirstEvent = true;
};
