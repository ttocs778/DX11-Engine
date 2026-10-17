#pragma once
#include "assetCache.h"
#include "animation.h"
#include "material.h"
#include "mesh.h"
#include "Shader.h"
#include "skeleton.h"
#include "texture.h"
#include <string>

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    static AssetManager* Get() { return s_theManager; }

    void Clear();

    Shader* GetShader(const std::wstring& shaderName);
    void SetShader(const std::wstring& shaderName, Shader* pShader);

    Texture* LoadTexture(const std::wstring& fileName);
    Material* LoadMaterial(const std::wstring& materialName);
    Mesh* LoadMesh(const std::wstring& meshName);
    Skeleton* LoadSkeleton(const std::wstring& skelName);
    Animation* LoadAnimation(const std::wstring& animName);

private:
    AssetCache<Shader> mShaders;
    AssetCache<Texture> mTextures;
    AssetCache<Material> mMaterials;
    AssetCache<Mesh> mMeshes;
    AssetCache<Skeleton> mSkeletons;
    AssetCache<Animation> mAnims;

    static AssetManager* s_theManager;
};
