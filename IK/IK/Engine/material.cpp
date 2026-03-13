#include "stdafx.h"
#include "material.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "Shader.h"
#include "stringUtil.h"
#include "texture.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>

Material::Material()
    : mBuffer(nullptr)
    , mShader(nullptr)
{
    mBuffer = Graphics::Get()->CreateGraphicsBuffer(&mConst, sizeof(mConst), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
    for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; ++i)
    {
        mTexture[i] = nullptr;
    }
}

Material::~Material()
{
    mBuffer->Release();
}

void Material::SetActive()
{
    Graphics* pGraphics = Graphics::Get();
    mShader->SetActive();
    for (int i = 0; i < Graphics::TEXTURE_SLOT_TOTAL; ++i)
    {
        if (nullptr != mTexture[i])
            mTexture[i]->SetActive(i);
        else
            pGraphics->SetActiveTexture(i, nullptr);
    }
    pGraphics->UploadBuffer(mBuffer, &mConst, sizeof(mConst));
    pGraphics->GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_MATERIAL, 1, &mBuffer);
}

void Material::SetShader(const Shader* shader)
{
    mShader = shader;
}

void Material::SetTexture(int slot, const Texture* texture)
{
    mTexture[slot] = texture;
}

void Material::SetDiffuseColor(const Vector3& color)
{
    mConst.c_diffuseColor = color;
}

void Material::SetSpecularColor(const Vector3& color)
{
    mConst.c_specularColor = color;
}

void Material::SetSpecularPower(float power)
{
    mConst.c_specularPower = power;
}

bool Material::Load(const WCHAR* fileName, AssetManager* pAssetManager)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject())
    {
        return false;
    }

    std::string str = doc["metadata"]["type"].GetString();
    int ver = doc["metadata"]["version"].GetInt();

    // Check the metadata
    if (!doc["metadata"].IsObject() ||
        str != "itpmat" ||
        ver != 1)
    {
        return false;
    }

    // Load Shader
    std::wstring shaderName;
    if (false == GetWStringFromJSON(doc, "shader", shaderName))
        return false;
    mShader = pAssetManager->GetShader(shaderName);
    DbgAssert(nullptr != mShader, "Material unable to load shader");

    {	// Load textures
        const rapidjson::Value& textures = doc["textures"];
        if (textures.IsArray())
        {
            for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
            {
                if (i < Graphics::TEXTURE_SLOT_TOTAL)
                {
                    std::wstring textureName;
                    StringUtil::String2WString(textureName,
                        textures[i].GetString());
                    mTexture[i] = pAssetManager->LoadTexture(textureName);
                }
            }
        }
    }

    // Load the Lighting Parameters
    GetVectorFromJSON(doc, "diffuseColor", mConst.c_diffuseColor);
    GetVectorFromJSON(doc, "specularColor", mConst.c_specularColor);
    GetFloatFromJSON(doc, "specularPower", mConst.c_specularPower);

    return true;
}

/*static*/ Material* Material::StaticLoad(const WCHAR* fileName, AssetManager* pManager)
{
    Material* pMat = new Material();
    pMat->Load(fileName, pManager);
    return pMat;
}
