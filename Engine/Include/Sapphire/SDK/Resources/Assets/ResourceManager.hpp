// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_RESOURCE_MANAGER_GUARD
#define SAPPHIRE_SDK_RESOURCE_MANAGER_GUARD

#include <unordered_map>

#include <Collections/Debug>

#include <Core/Types/IInterface.hpp>

#include <SDK/Resources/Assets/AssetDependency.hpp>

namespace Sa
{
	class AssetManager;

	struct ResourceInfos
	{
		std::string path;
		std::vector<AssetDependency> dependencies;
	};


	class IResourceMgrBase : public IInterface
	{
	public:
		virtual void Save(const void* _asset, const std::string& _newPath) = 0;
	};


	template<typename T, typename AssetT>
	class ResourceMgr : public IResourceMgrBase
	{
		struct SmartPtr
		{
			T* ptr = nullptr;
			int32 count = 0u;
		};

		std::unordered_map<std::string, SmartPtr> mPathToResource;
		std::unordered_map<T*, ResourceInfos> mResourceToInfos;

		AssetManager& mAssetMgr;

		void SaveHelper(AssetT& _asset);
		void SaveHelper(std::vector<AssetT>& _assets);

	public:
		ResourceMgr(AssetManager& _assetMgr) noexcept;
		
		ResourceMgr(ResourceMgr&&) = delete;
		ResourceMgr(const ResourceMgr&) = delete;

		AssetManager& GetAssetMgr() const;

		T* Load(const AssetT& _asset, bool _bPreLoaded = false);
		std::vector<T*> Load(const std::vector<AssetT>& _assets, bool _bPreLoaded = false);
		T* Load(const std::string& _assetPath, bool _bPreLoaded = false);

		void Unload(T* _resource);
		void Unload(const std::vector<T*>& _resources);

		typename AssetT::ImportResultT Import(const std::string& _resourcePath, const typename AssetT::ImportInfosT& _importInfos);
		AssetT Create(typename AssetT::CreateInfosT&& _createInfos);

		void Save(const AssetT& _asset, const std::string& _newPath);
		void Save(const void* _asset, const std::string& _newPath) override final;

		T* Query(const std::string& _filePath) const;

		void Clear();


		ResourceMgr& operator=(ResourceMgr&&) = delete;
		ResourceMgr& operator=(const ResourceMgr&) = delete;
	};
}

#include <SDK/Resources/Assets/ResourceManager.inl>

#endif // GUARD
