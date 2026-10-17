#pragma once
#include "assetCache.h"
#include "Shader.h"
#include "texture.h"
#include "Material.h"
#include "mesh.h"
#include "Skeleton.h"
#include "Animation.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();
	Shader* GetShader(const std::wstring& shaderName);
	void SetShader(const std::wstring& shaderName, Shader* pShader);
	Texture* LoadTexture(const std::wstring& fileName);
	Material* LoadMaterial(const std::wstring& materialName);
	Mesh* LoadMesh(const std::wstring& fileName);
	Skeleton* LoadSkeleton(const std::wstring& fileName);
	Animation* LoadAnimation(const std::wstring& fileName);
	void Clear();

private:
	AssetCache<Shader>* mAssetShaderCache = new AssetCache<Shader>(this);
	AssetCache<Texture>* mAssetTextureCache = new AssetCache<Texture>(this);
	AssetCache<Material>* mAssetMaterialeCache = new AssetCache<Material>(this);
	AssetCache<Mesh>* mAssetMeshCache = new AssetCache<Mesh>(this);
	AssetCache<Skeleton>* mAssetSkeletonCache = new AssetCache<Skeleton>(this);
	AssetCache<Animation>* mAssetAnimationCache = new AssetCache<Animation>(this);
};
