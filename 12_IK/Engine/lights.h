#pragma once
#include "engineMath.h"

namespace Lights
{
    constexpr int MAX_POINT_LIGHTS = 8;

    struct PointLightData
    {
	    Vector3 lightColor;
        float pad0;

	    Vector3 position;
	    float innerRadius;

	    float outerRadius;
	    bool isEnabled;
        float pad1, pad2;
    };

    struct LightingConstants
    {
	    Vector3 c_ambient;
        float pad;
	    PointLightData c_pointLight[MAX_POINT_LIGHTS];
    };
}