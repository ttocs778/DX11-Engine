#pragma once
#include "engineMath.h"
#include "Graphics.h"

class AssetManager;
class Shader;
class Texture;

class Material
{
public:
    struct MaterialConstants
    {
	    Vector3 c_diffuseColor;
        float pad;
	    Vector3 c_specularColor;
	    float c_specularPower;
    };

    Material();
    ~Material();
    void SetActive();
    void SetShader(const Shader* shader);
    void SetTexture(int slot, const Texture* texture);
    void SetDiffuseColor(const Vector3& color);
    void SetSpecularColor(const Vector3& color);
    void SetSpecularPower(float power);

    bool Load(const WCHAR* fileName, AssetManager* pAssetManager);
    static Material* StaticLoad(const WCHAR* fileName, AssetManager* pManager);

    MaterialConstants mConst;

private:
    ID3D11Buffer* mBuffer;
    const Shader* mShader;
    const Texture* mTexture[Graphics::TEXTURE_SLOT_TOTAL];
};
