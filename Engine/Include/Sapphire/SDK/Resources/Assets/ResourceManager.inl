// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template<typename T, typename AssetT>
	ResourceMgr<T, AssetT>::ResourceMgr(AssetManager& _assetMgr) noexcept : mAssetMgr{ _assetMgr }
	{
	}

	template<typename T, typename AssetT>
	AssetManager& ResourceMgr<T, AssetT>::GetAssetMgr() const
	{
		return mAssetMgr;
	}

	template<typename T, typename AssetT>
	T* ResourceMgr<T, AssetT>::Load(const AssetT& _asset, bool _bPreLoaded)
	{
		if (!_asset.IsValid())
			return nullptr;

		const std::string& assetPath = _asset.GetFilePath();
		SA_ASSERT(!assetPath.empty(), InvalidParam, SDK_Asset, L"Load from empty path!");

		auto& smartPtr = mPathToResource[assetPath];

		// Not already loaded.
		if (!smartPtr.ptr)
		{
			// Load dependencies.
			std::vector<AssetDependency> dependencies;
			const std::vector<AssetPathDependency> pathDependencies = _asset.GetPathDependencies();

			for (auto it = pathDependencies.begin(); it != pathDependencies.end(); ++it)
				dependencies.emplace_back(AssetDependency{ mAssetMgr.Load(it->path, it->assetType, false), it->assetType });


			// Create resource.
			smartPtr.ptr = _asset.Create(mAssetMgr.GetRenderInstance());
			mResourceToInfos[smartPtr.ptr] = ResourceInfos{ assetPath, Move(dependencies) };
		}

		// Preload does not increment count.
		if (!_bPreLoaded)
			++smartPtr.count;

		return smartPtr.ptr;
	}

	template<typename T, typename AssetT>
	std::vector<T*> ResourceMgr<T, AssetT>::Load(const std::vector<AssetT>& _assets, bool _bPreLoaded)
	{
		std::vector<T*> result;
		result.reserve(_assets.size());

		for (uint32 i = 0u; i < _assets.size(); ++i)
		{
			if (_assets[i].IsValid())
				result.push_back(Load(_assets[i], _bPreLoaded));
		}

		return result;
	}

	template<typename T, typename AssetT>
	T* ResourceMgr<T, AssetT>::Load(const std::string& _assetPath, bool _bPreLoaded)
	{
		SA_ASSERT(!_assetPath.empty(), InvalidParam, SDK_Asset, L"Load from empty path!");

		auto it = mPathToResource.find(_assetPath);

		// Already loaded.
		if (it != mPathToResource.end() && it->second.ptr)
			return it->second.ptr;

		AssetT asset(*this);

		// Can't load asset: file does not exist.
		if (!asset.Load(_assetPath))
			return nullptr;

		return Load(asset, _bPreLoaded);
	}


	template<typename T, typename AssetT>
	void ResourceMgr<T, AssetT>::Unload(T* _resource)
	{
		SA_ASSERT(_resource, Nullptr, SDK_Asset, L"Try unload nullptr resource!");

		auto infosIt = mResourceToInfos.find(_resource);
		
		// Resource not foud.
		if (infosIt != mResourceToInfos.end())
			return;

		const ResourceInfos& infos = infosIt->second;

		auto it = mPathToResource.find(infos.path);
		SA_ASSERT(it != mPathToResource.end(), InvalidParam, SDK_Asset, L"Resource path does not exist!");

		auto& smartPtr = it->second;

		if (--smartPtr.count <= 0)
		{
			// Unload dependencies.
			for (auto it = infos.dependencies.begin(); it != infos.dependencies.end(); ++it)
				mAssetMgr.UnLoad(it->primitive, it->assetType);

			// Destroy resource.
			smartPtr.ptr->Destroy(mAssetMgr.GetRenderInstance());
			delete smartPtr.ptr;

			mPathToResource.erase(it);
			mResourceToInfos.erase(infosIt);
		}
	}

	template<typename T, typename AssetT>
	void ResourceMgr<T, AssetT>::Unload(const std::vector<T*>& _resources)
	{
		for (auto it = _resources.begin(); it != _resources.end(); ++it)
			Unload(it);
	}


	template<typename T, typename AssetT>
	typename AssetT::ImportResultT ResourceMgr<T, AssetT>::Import(const std::string& _resourcePath,
		const typename AssetT::ImportInfosT& _importInfos)
	{
		typename AssetT::ImportResultT assets = AssetT::Import(*this, _resourcePath, _importInfos);

		SaveHelper(assets);

		if (_importInfos.bKeepLoaded)
			Load(assets, true);

		return assets;
	}

	template<typename T, typename AssetT>
	AssetT ResourceMgr<T, AssetT>::Create(typename AssetT::CreateInfosT&& _createInfos)
	{
		AssetT asset(*this, Move(_createInfos));

		asset.Save();

		if (_createInfos.bKeepLoaded)
			Load(asset);

		return asset;
	}

	template<typename T, typename AssetT>
	void ResourceMgr<T, AssetT>::Save(const AssetT& _asset, const std::string& _oldPath)
	{
		auto oldIt = !_oldPath.empty() ? mPathToResource.find(_oldPath) : mPathToResource.end();

		if (oldIt != mPathToResource.end())
			Unload(oldIt->second.ptr);

		// Pre-load.
		Load(_asset, true);
	}

	template<typename T, typename AssetT>
	void ResourceMgr<T, AssetT>::Save(const void* _asset, const std::string& _newPath)
	{
		SA_ASSERT(_asset, Nullptr, SDK_Asset, L"Asset nullptr!")

		Save(*reinterpret_cast<const AssetT*>(_asset), _newPath);
	}

	template<typename T, typename AssetT>
	void ResourceMgr<T, AssetT>::SaveHelper(AssetT& _asset)
	{
		if (_asset.IsValid())
			_asset.Save("", false);
	}

	template<typename T, typename AssetT>
	void ResourceMgr<T, AssetT>::SaveHelper(std::vector<AssetT>& _assets)
	{
		for (auto it = _assets.begin(); it != _assets.end(); ++it)
		{
			if (it->IsValid())
				it->Save("", false);
		}
	}

	template<typename T, typename AssetT>
	T* ResourceMgr<T, AssetT>::Query(const std::string& _filePath) const
	{
		auto it = mPathToResource.find(_filePath);

		if (it != mPathToResource.end())
			return it->second.ptr;

		return nullptr;
	}

	template<typename T, typename AssetT>
	void ResourceMgr<T, AssetT>::Clear()
	{
		for (auto it = mPathToResource.begin(); it != mPathToResource.end(); ++it)
		{
			SA_ASSERT(it->second.ptr, Nullptr, SDK_Asset, L"Destroy nullptr asset!");
			
			it->second.ptr->Destroy(mAssetMgr.GetRenderInstance());
			delete it->second.ptr;
		}

		mPathToResource.clear();
		mResourceToInfos.clear();
	}
}
