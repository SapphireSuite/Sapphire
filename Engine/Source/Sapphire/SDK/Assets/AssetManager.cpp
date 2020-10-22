// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/AssetManager.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	AssetManager::AssetManager(const IRenderInstance& _instance) noexcept :
		mInstance{ _instance },
		importer{ *this },
		meshMgr{ *this },
		shaderMgr{ *this },
		textureMgr{ *this },
		cubemapMgr{ *this },
		renderMatMgr{ *this }
	{
	}

	AssetManager::~AssetManager()
	{
		Clear();
	}

	const IRenderInstance& AssetManager::GetRenderInstance() const
	{
		return mInstance;
	}

	IRenderPrimitive* AssetManager::Load(const std::string& _filePath, bool _bKeepLoaded)
	{
		AssetType assetType = IAsset::GetAssetTypeFromAsset(_filePath);

		return Load(_filePath, assetType, _bKeepLoaded);
	}

	IRenderPrimitive* AssetManager::Load(const std::string& _filePath, AssetType _assetType, bool _bKeepLoaded)
	{
		switch (_assetType)
		{
			case Sa::AssetType::Mesh:
				return meshMgr.Load(_filePath, _bKeepLoaded);
			case Sa::AssetType::Shader:
				return shaderMgr.Load(_filePath, _bKeepLoaded);
			case Sa::AssetType::Texture:
				return textureMgr.Load(_filePath, _bKeepLoaded);
			case Sa::AssetType::Cubemap:
				return cubemapMgr.Load(_filePath, _bKeepLoaded);
			case Sa::AssetType::RenderMaterial:
				return renderMatMgr.Load(_filePath, _bKeepLoaded);
			default:
			{
				SA_LOG("Wrong asset type!", Error, SDK_Asset);
				return nullptr;
			}
		}
	}

	void AssetManager::UnLoad(IRenderPrimitive* _primitive)
	{
		if (dynamic_cast<IMesh*>(_primitive))
			UnLoad(_primitive, AssetType::Mesh);
		else if (dynamic_cast<IShader*>(_primitive))
			UnLoad(_primitive, AssetType::Shader);
		else if (dynamic_cast<ITexture*>(_primitive))
			UnLoad(_primitive, AssetType::Texture);
		else if (dynamic_cast<IRenderMaterial*>(_primitive))
			UnLoad(_primitive, AssetType::RenderMaterial);
		else
			SA_LOG("Wrong asset type!", Error, SDK_Asset);
	}

	void AssetManager::UnLoad(IRenderPrimitive* _primitive, AssetType _assetType)
	{
		SA_ASSERT(_primitive, Nullptr, SDK_Asset, L"Try to unload nullptr primitive!");

		switch (_assetType)
		{
			case Sa::AssetType::Texture:
				textureMgr.Unload(&_primitive->As<ITexture>());
				break;
			case Sa::AssetType::Shader:
				shaderMgr.Unload(&_primitive->As<IShader>());
				break;
			case Sa::AssetType::RenderMaterial:
				renderMatMgr.Unload(&_primitive->As<IRenderMaterial>());
				break;
			case Sa::AssetType::Mesh:
				meshMgr.Unload(&_primitive->As<IMesh>());
				break;
			default:
				SA_LOG("Wrong asset type!", Warning, SDK_Asset);
		}
	}

	void AssetManager::Clear()
	{
		meshMgr.Clear();
		shaderMgr.Clear();
		textureMgr.Clear();
		renderMatMgr.Clear();
	}
}