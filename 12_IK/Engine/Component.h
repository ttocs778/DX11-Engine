#pragma once
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"


class RenderObj;

class Component
{
public:
    Component(RenderObj* renderObj);
    virtual ~Component();

    virtual void LoadProperties(const rapidjson::Value& properties);
    virtual void Update(float deltaTime);

protected:
    RenderObj* mOwner;
};