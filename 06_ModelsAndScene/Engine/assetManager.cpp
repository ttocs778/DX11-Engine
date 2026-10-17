#include "stdafx.h"
#include "assetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
	Clear();
}


Shader* AssetManager::GetShader(const std::wstring& shaderName)
{
	return mAssetShaderCache->Get(shaderName);
}

void AssetManager::SetShader(const std::wstring& shaderName, Shader* pShader)
{
	mAssetShaderCache->Cache(shaderName, pShader);
}

Texture* AssetManager::LoadTexture(const std::wstring& fileName)
{
	return mAssetTextureCache->Load(fileName);
}

Material* AssetManager::LoadMaterial(const std::wstring& materialName)
{
	return mAssetMaterialeCache->Load(materialName);
}

Mesh* AssetManager::LoadMesh(const std::wstring& fileName)
{
	return mAssetMeshCache->Load(fileName);

}

void AssetManager::Clear()
{
	if (mAssetShaderCache) {
		delete mAssetShaderCache;
		mAssetShaderCache = nullptr;
	}
	if (mAssetTextureCache) {
		delete mAssetTextureCache;
		mAssetTextureCache = nullptr;
	}
	if (mAssetMaterialeCache) {
		delete mAssetMaterialeCache;
		mAssetMaterialeCache = nullptr;
	}
	if (mAssetMeshCache) {
		delete mAssetMeshCache;
		mAssetMeshCache = nullptr;
	}
}
