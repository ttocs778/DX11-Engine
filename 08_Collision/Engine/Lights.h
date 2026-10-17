#pragma once
namespace Lights
{
	struct PointLightData
	{
		Vector3 lightColor; // 12 bytes
		float  padding1;    // 4 bytes

		Vector3 position;   // 12 bytes
		float  innerRadius; // 4 bytes
		float  outerRadius; // 4 bytes
		bool   isEnabled;   // 1 bytes
		char padding2; // 1 byte
		short padding3; // 2 bytes
		float padding4; // 4 bytes
		float padding5; // 4 bytes
	};

	constexpr int pointLightNum = 8;

	struct LightingConstants
	{
		Vector3 c_ambient; // 12 bytes
		float   padding4;  // 4 bytes (align next array element to 16 bytes)

		PointLightData c_pointLight[pointLightNum];
	};
}


