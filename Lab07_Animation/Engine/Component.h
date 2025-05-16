#pragma once
#include "jsonUtil.h"

class RenderObj;

class Component
{
public:
	Component(RenderObj* pObj) : mOwner(pObj) {}

	virtual ~Component() {}

	virtual void LoadProperties(const rapidjson::Value& properties) {}

	virtual void Update(float deltaTime) {}

protected:
	RenderObj* mOwner;
};