#include "stdafx.h"
#include "Component.h"
#include "RenderObj.h"


Component::Component(RenderObj* renderObj)
    : mOwner(renderObj)
{
}

/*virtual*/ Component::~Component()
{
}

/*virtual*/ void Component::LoadProperties(const rapidjson::Value& properties)
{
}

/*virtual*/ void Component::Update(float deltaTime)
{
}
