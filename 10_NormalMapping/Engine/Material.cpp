#include "stdafx.h"
#include "Material.h"
#include <fstream>
#include <sstream>
#include "jsonUtil.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include "stringUtil.h"
#include "assetManager.h"

Material::Material() {
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MaterialConstants);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&bd, nullptr, &mMaterialBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create Material constant buffer\n");
		mMaterialBuffer = nullptr;
	}
	mMaterialConstants.c_specularPower = 10.0f;
}

Material::~Material()
{
	if (mMaterialBuffer)
	{
		mMaterialBuffer->Release();
		mMaterialBuffer = nullptr;
	}
}


void Material::SetActive()
{
	mMaterialShader->SetActive();
	for (int i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i])
		{
			mTextures[i]->SetActive(i);
		}
		else
		{
			//OutputDebugString(L"Material::SetActive: Texture slot is nullptr.\n");
		}
	}

	Graphics::Get()->GetDeviceContext()->UpdateSubresource(mMaterialBuffer, 0, nullptr, &mMaterialConstants, 0, 0);
	Graphics::Get()->GetDeviceContext()->PSSetConstantBuffers(Graphics::CONSTANT_BUFFER_MATERIAL, 1, &mMaterialBuffer);

}

void Material::SetShader(const Shader* shader)
{
	mMaterialShader = shader;
}
void Material::SetTexture(int slot, const Texture* texture)
{
	mTextures[slot] = texture;
}
void Material::SetDiffuseColor(const Vector3& color)
{
	mMaterialConstants.c_diffuseColor = color;

}
void Material::SetSpecularColor(const Vector3& color)
{
	mMaterialConstants.c_specularColor = color;

}
void Material::SetSpecularPower(float power)
{
	mMaterialConstants.c_specularPower = power;

}
Material* Material::StaticLoad(const WCHAR* fileName, AssetManager* pManager)
{
	Material* pMaterial = new Material();
	if (false == pMaterial->Load(fileName, pManager))
	{
		return pMaterial;
	}
	return pMaterial;
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
	mMaterialShader = pAssetManager->GetShader(shaderName);
	DbgAssert(nullptr != mMaterialShader, "Material unable to load shader");
	{ // Load textures
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
					mTextures[i] = pAssetManager->LoadTexture(textureName);
				}
			}
		}
	}
	// Load the Lighting Parameters
	GetVectorFromJSON(doc, "diffuseColor", mMaterialConstants.c_diffuseColor);
	GetVectorFromJSON(doc, "specularColor", mMaterialConstants.c_specularColor);
	GetFloatFromJSON(doc, "specularPower", mMaterialConstants.c_specularPower);
	return true;
}
