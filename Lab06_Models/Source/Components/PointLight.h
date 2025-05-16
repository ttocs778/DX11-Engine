#pragma once

#include "Component.h"  
#include "Lights.h"               
#include "Game.h"                  
#include "jsonUtil.h"

class PointLight : public Component
{
public:
	PointLight(RenderObj* pObj, Game* pGame);

	virtual ~PointLight() override;

	virtual void LoadProperties(const rapidjson::Value& properties) override;

	virtual void Update(float deltaTime) override;

private:
	Lights::PointLightData* mLightData;
	Game* mGame;
};
