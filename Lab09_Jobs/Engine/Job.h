#pragma once
class Job {
public:
	virtual ~Job() {}
	virtual void DoIt() = 0;
};

