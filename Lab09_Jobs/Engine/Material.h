#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "Texture.h"
#include "Shader.h"
#include "array"
struct MaterialConstants {
	Vector3 c_diffuseColor;
	float   padding1;
	Vector3 c_specularColor;
	float   c_specularPower;

};

class Material
{
public:
	Material();
	~Material();
	void SetActive();
	void SetShader(const Shader* shader);
	void SetTexture(int slot, const Texture* texture);
	void SetDiffuseColor(const Vector3& color);
	void SetSpecularColor(const Vector3& color);
	void SetSpecularPower(float power);
	static Material* StaticLoad(const WCHAR* fileName, AssetManager* pManager);
	MaterialConstants mMaterialConstants;
	bool Load(const WCHAR* fileName, AssetManager* pAssetManager);
protected:
	ID3D11Buffer* mMaterialBuffer;
	const Shader* mMaterialShader;
	std::array<const Texture*, Graphics::TEXTURE_SLOT_TOTAL> mTextures;
};


