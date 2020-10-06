// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/AssetManager.hpp>

#include <Core/Algorithms/Move.hpp>

namespace Sa
{
	AssetManager::AssetManager(const IRenderInstance& _instance, const IRenderSurface& _surface) noexcept :
		mInstance{ _instance },
		mSurface{ _surface }
	{
	}

	AssetManager::~AssetManager()
	{
		Clear();
	}


	IMesh* AssetManager::QueryMesh(const std::string& _filePath)
	{
		return mMeshes[_filePath].ptr;
	}

	IMesh* AssetManager::LoadMesh(const std::string& _filePath, bool _bPreLoaded)
	{
		auto& pair = mMeshes[_filePath];

		// Not already loaded.
		if (!pair.ptr)
		{
			MeshAsset asset(*this);

			// Can't load asset: file does not exist.
			if (!asset.Load(_filePath))
				return nullptr;

			pair.ptr = asset.Create(mInstance);
		}

		// Preload does not increment count.
		if (!_bPreLoaded)
			++pair.count;

		return pair.ptr;
	}

	void AssetManager::UnloadMesh(IMesh* _mesh)
	{
	}

	std::vector<MeshAsset> AssetManager::ImportMeshes(const std::string& _resourcePath, const MeshImportInfos& _importInfos)
	{
		std::vector<MeshAsset> assets = MeshAsset::Import(*this, _resourcePath, _importInfos);

		for (uint32 i = 0u; i < assets.size(); ++i)
		{
			auto& pair = mMeshes[_importInfos.outFilePaths[i]];

			// Not already imported.
			if (_importInfos.bKeepLoaded)
			{
				if(!pair.ptr)
					pair.ptr = assets[i].Create(mInstance);
			}

		}

		return assets;
	}

	std::vector<MeshAsset> AssetManager::ReImportMeshes(const std::string& _resourcePath, const MeshImportInfos& _importInfos)
	{
		for (auto it = _importInfos.outFilePaths.begin(); it != _importInfos.outFilePaths.end(); ++it)
		{
			auto& pair = mMeshes[*it];

			// Already imported: reimport.
			if (pair.ptr)
			{
				pair.ptr->Destroy(mInstance);
				delete pair.ptr;

				pair.ptr = nullptr;
			}
		}

		return ImportMeshes(_resourcePath, _importInfos);
	}

	MeshAsset AssetManager::CreateMesh(MeshCreateInfos&& _createInfos)
	{
		auto& pair = mMeshes[_createInfos.outFilePaths];

		MeshAsset asset(*this, Move(_createInfos.vertices), Move(_createInfos.indices));

		if(!pair.ptr || _createInfos.bErase)
		{
			asset.Save(_createInfos.outFilePaths);

			if (_createInfos.bKeepLoaded)
				pair.ptr = asset.Create(mInstance);
		}

		return asset;
	}

	void AssetManager::SaveMesh(const MeshAsset& _asset, const std::string& _newPath)
	{
		const std::string& oldPath = _asset.GetFilePath();

		auto& pair = mMeshes[oldPath];

		if (pair.ptr)
		{
			pair.ptr->Destroy(mInstance);
			delete pair.ptr;

			if (oldPath != _newPath)
			{
				mMeshes.erase(oldPath);

				pair = mMeshes[_newPath];
			}
		}

		pair.ptr = _asset.Create(mInstance); // Pre-load.
	}


	IShader* AssetManager::QueryShader(const std::string& _filePath)
	{
		return mShaders[_filePath].ptr;
	}

	IShader* AssetManager::LoadShader(const std::string& _filePath, bool _bPreLoaded)
	{
		auto& pair = mShaders[_filePath];

		// Not already loaded.
		if (!pair.ptr)
		{
			ShaderAsset asset(*this);

			// Can't load asset: file does not exist.
			if (!asset.Load(_filePath))
				return nullptr;

			pair.ptr = asset.Create(mInstance);
		}

		// Preload does not increment count.
		if (!_bPreLoaded)
			++pair.count;

		return pair.ptr;
	}

	void AssetManager::UnloadShader(IShader* _shader)
	{
	}

	ShaderAsset AssetManager::ImportShader(const std::string& _resourcePath, const ShaderImportInfos& _importInfos)
	{
		auto& pair = mShaders[_importInfos.outFilePath];

		ShaderAsset asset(*this);

		// Not already imported.
		if (!pair.ptr)
		{
			asset = ShaderAsset::Import(*this, _resourcePath, _importInfos);

			if (_importInfos.bKeepLoaded)
				pair.ptr = asset.Create(mInstance);
		}

		return asset;
	}

	ShaderAsset AssetManager::ReImportShader(const std::string& _resourcePath, const ShaderImportInfos& _importInfos)
	{
		auto& pair = mShaders[_importInfos.outFilePath];

		// Already imported: reimport.
		if (pair.ptr)
		{
			pair.ptr->Destroy(mInstance);
			delete pair.ptr;

			pair.ptr = nullptr;
		}

		return ImportShader(_resourcePath, _importInfos);
	}

	void AssetManager::SaveShader(const ShaderAsset& _asset, const std::string& _newPath)
	{
		const std::string& oldPath = _asset.GetFilePath();

		auto& pair = !oldPath.empty() ? mShaders[oldPath] : mShaders[_newPath];

		if (pair.ptr)
		{
			pair.ptr->Destroy(mInstance);
			delete pair.ptr;

			if (oldPath != _newPath)
			{
				mShaders.erase(oldPath);

				pair = mShaders[_newPath];
			}
		}

		pair.ptr = _asset.Create(mInstance); // Pre-load.
	}


	ITexture* AssetManager::QueryTexture(const std::string& _filePath)
	{
		return mTextures[_filePath].ptr;
	}

	ITexture* AssetManager::LoadTexture(const std::string& _filePath, bool _bPreLoaded)
	{
		auto& pair = mTextures[_filePath];

		// Not already loaded.
		if (!pair.ptr)
		{
			TextureAsset asset(*this);

			// Can't load asset: file does not exist.
			if (!asset.Load(_filePath))
				return nullptr;

			pair.ptr = asset.Create(mInstance);
		}

		// Preload does not increment count.
		if (!_bPreLoaded)
			++pair.count;

		return pair.ptr;
	}

	void AssetManager::UnloadTexture(ITexture* _texture)
	{
	}

	TextureAsset AssetManager::ImportTexture(const std::string& _resourcePath, const TextureImportInfos& _importInfos)
	{
		auto& pair = mTextures[_importInfos.outFilePath];

		TextureAsset asset(*this);

		// Not already imported.
		if (!pair.ptr)
		{
			asset = TextureAsset::Import(*this, _resourcePath, _importInfos);

			if (_importInfos.bKeepLoaded)
				pair.ptr = asset.Create(mInstance);
		}

		return asset;
	}

	TextureAsset AssetManager::ReImportTexture(const std::string& _resourcePath, const TextureImportInfos& _importInfos)
	{
		auto& pair = mTextures[_importInfos.outFilePath];

		// Already imported: reimport.
		if (pair.ptr)
		{
			pair.ptr->Destroy(mInstance);
			delete pair.ptr;

			pair.ptr = nullptr;
		}

		return ImportTexture(_resourcePath, _importInfos);
	}

	void AssetManager::SaveTexture(const TextureAsset& _asset, const std::string& _newPath)
	{
		const std::string& oldPath = _asset.GetFilePath();

		auto& pair = mTextures[oldPath];

		if (pair.ptr)
		{
			pair.ptr->Destroy(mInstance);
			delete pair.ptr;

			if (oldPath != _newPath)
			{
				mTextures.erase(oldPath);

				pair = mTextures[_newPath];
			}
		}

		pair.ptr = _asset.Create(mInstance); // Pre-load.
	}


	IRenderMaterial* AssetManager::QueryMaterial(const std::string& _filePath)
	{
		return mMaterials[_filePath].ptr;
	}

	IRenderMaterial* AssetManager::LoadMaterial(const std::string& _filePath, bool _bPreLoaded)
	{
		auto& pair = mMaterials[_filePath];

		// Not already loaded.
		if (!pair.ptr)
		{
			MaterialAsset asset(*this);

			// Can't load asset: file does not exist.
			if (!asset.Load(_filePath))
				return nullptr;

			pair.ptr = asset.Create(mInstance, mSurface);
		}

		// Preload does not increment count.
		if (!_bPreLoaded)
			++pair.count;

		return pair.ptr;
	}

	void AssetManager::UnloadMaterial(IRenderMaterial* _material)
	{
	}

	std::vector<MaterialAsset> AssetManager::ImportMaterials(const std::string& _resourcePath, const MaterialImportInfos& _importInfos)
	{
		std::vector<MaterialAsset> assets = MaterialAsset::Import(*this, _resourcePath, _importInfos);

		for (uint32 i = 0u; i < assets.size(); ++i)
		{
			auto& pair = mMaterials[_importInfos.outFilePaths[i]];

			// Not already imported.
			if (_importInfos.bKeepLoaded)
			{
				if (!pair.ptr && assets[i].IsValid())
					pair.ptr = assets[i].Create(mInstance, mSurface);
			}

		}

		return assets;
	}

	std::vector<MaterialAsset> AssetManager::ReImportMaterials(const std::string& _resourcePath, const MaterialImportInfos& _importInfos)
	{
		for (auto it = _importInfos.outFilePaths.begin(); it != _importInfos.outFilePaths.end(); ++it)
		{
			auto& pair = mMaterials[*it];

			// Already imported: reimport.
			if (pair.ptr)
			{
				pair.ptr->DestroyPipeline(mInstance);
				delete pair.ptr;

				pair.ptr = nullptr;
			}
		}

		return ImportMaterials(_resourcePath, _importInfos);
	}

	MaterialAsset AssetManager::CreateMaterial(MaterialCreateInfos&& _createInfos)
	{
		auto& pair = mMaterials[_createInfos.outFilePaths];

		// Save output file path before Move.
		std::string outFilePath = _createInfos.outFilePaths;

		MaterialAsset asset(*this, Move(_createInfos));

		if (!pair.ptr || _createInfos.bErase)
		{
			asset.Save(outFilePath);

			if (_createInfos.bKeepLoaded)
				pair.ptr = asset.Create(mInstance, mSurface);
		}

		return asset;
	}

	void AssetManager::SaveMaterial(const MaterialAsset& _asset, const std::string& _newPath)
	{
		const std::string& oldPath = _asset.GetFilePath();

		auto& pair = mMaterials[oldPath];

		if (pair.ptr)
		{
			pair.ptr->DestroyPipeline(mInstance);
			delete pair.ptr;

			if (oldPath != _newPath)
			{
				mMaterials.erase(oldPath);

				pair = mMaterials[_newPath];
			}
		}

		pair.ptr = _asset.Create(mInstance, mSurface); // Pre-load.
	}


	void AssetManager::Clear()
	{
		for (auto it = mMaterials.begin(); it != mMaterials.end(); ++it)
		{
			SA_ASSERT(it->second.ptr, Nullptr, SDK_Asset, L"Destroy nullptr asset!");

			delete it->second.ptr;
		}

		for (auto it = mMeshes.begin(); it != mMeshes.end(); ++it)
		{
			SA_ASSERT(it->second.ptr, Nullptr, SDK_Asset, L"Destroy nullptr asset!");

			delete it->second.ptr;
		}

		for (auto it = mShaders.begin(); it != mShaders.end(); ++it)
		{
			SA_ASSERT(it->second.ptr, Nullptr, SDK_Asset, L"Destroy nullptr asset!");

			delete it->second.ptr;
		}

		for (auto it = mTextures.begin(); it != mTextures.end(); ++it)
		{
			SA_ASSERT(it->second.ptr, Nullptr, SDK_Asset, L"Destroy nullptr asset!");

			delete it->second.ptr;
		}
	}
}