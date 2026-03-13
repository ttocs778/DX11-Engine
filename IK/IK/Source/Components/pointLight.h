#pragma once
#include "Component.h"
#include "lights.h"

class Game;

class PointLight : public Component
{
public:
    PointLight(RenderObj* renderObj, Game* game);
    ~PointLight();

    void LoadProperties(const rapidjson::Value& properties) override;
    void Update(float deltaTime) override;

private:
    Lights::PointLightData* mPointLight;
    Game* mGame;
};