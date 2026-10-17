#include "stdafx.h"
#include "assetManager.h"

/*static*/ AssetManager* AssetManager::s_theManager = nullptr;

AssetManager::AssetManager()
    : mShaders(this)
    , mTextures(this)
    , mMaterials(this)
    , mMeshes(this)
    , mSkeletons(this)
    , mAnims(this)
{
    DbgAssert(nullptr == s_theManager, "You can only have 1 AssetManager");
    s_theManager = this;
}

AssetManager::~AssetManager()
{
    s_theManager = nullptr;
}

void AssetManager::Clear()
{
    mShaders.Clear();
    mTextures.Clear();
    mMaterials.Clear();
    mMeshes.Clear();
    mSkeletons.Clear();
    mAnims.Clear();
}

Shader* AssetManager::GetShader(const std::wstring& shaderName)
{
    return mShaders.Get(shaderName);
}

void AssetManager::SetShader(const std::wstring& shaderName, Shader* pShader)
{
    mShaders.Cache(shaderName, pShader);
}

Texture* AssetManager::LoadTexture(const std::wstring& fileName)
{
    return mTextures.Load(fileName);
}

Material* AssetManager::LoadMaterial(const std::wstring& materialName)
{
    return mMaterials.Load(materialName);
}

Mesh* AssetManager::LoadMesh(const std::wstring& meshName)
{
    return mMeshes.Load(meshName);
}

Skeleton* AssetManager::LoadSkeleton(const std::wstring& skelName)
{
    return mSkeletons.Load(skelName);
}

Animation* AssetManager::LoadAnimation(const std::wstring& animName)
{
    return mAnims.Load(animName);
}
