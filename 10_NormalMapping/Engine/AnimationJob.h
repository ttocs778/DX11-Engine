#pragma once
#include "Job.h"

// Forward declaration to avoid circular dependency.
class Character;

class AnimationJob : public Job {
public:
	AnimationJob(Character* pCharacter);
	virtual void DoIt() override;
private:
	Character* mCharacter;
};

