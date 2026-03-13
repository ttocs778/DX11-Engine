#pragma once
#include "RenderObj.h"

class Material;

class RenderCube : public RenderObj
{
public:
    RenderCube(Material* material);
    virtual ~RenderCube();

private:
};