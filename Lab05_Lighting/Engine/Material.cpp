#include "stdafx.h"
#include "Material.h"


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
