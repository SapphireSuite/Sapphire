// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_RESOURCE_MANAGER_GUARD
#define SAPPHIRE_SDK_RESOURCE_MANAGER_GUARD

#include <unordered_map>

#include <Collections/Debug>

#include <Core/Types/IInterface.hpp>

#include <SDK/Assets/AssetDependency.hpp>

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


	template<typename AssetT>
	class ResourceMgr : public IResourceMgrBase
	{
	public:
		using ResT = typename AssetT::ResT;
		using RawT = typename AssetT::RawT;

	private:
		struct SmartPtr
		{
			ResT* ptr = nullptr;
			int32 count = 0u;
		};

		std::unordered_map<std::string, SmartPtr> mPathToResource;
		std::unordered_map<ResT*, ResourceInfos> mResourceToInfos;

		AssetManager& mAssetMgr;

		//void SaveHelper(AssetT& _asset);
		//void SaveHelper(std::vector<AssetT>& _assets);

	public:
		ResourceMgr(AssetManager& _assetMgr) noexcept;
		
		ResourceMgr(ResourceMgr&&) = delete;
		ResourceMgr(const ResourceMgr&) = delete;

		AssetManager& GetAssetMgr() const;

		ResT* Load(const AssetT& _asset, bool _bPreLoaded = false);
		std::vector<ResT*> Load(const std::vector<AssetT>& _assets, bool _bPreLoaded = false);
		ResT* Load(const std::string& _assetPath, bool _bPreLoaded = false);

		void Unload(ResT* _resource);
		void Unload(const std::vector<ResT*>& _resources);

		AssetT Create(RawT&& _createInfos);

		void Save(const AssetT& _asset, const std::string& _newPath);
		void Save(const void* _asset, const std::string& _newPath) override final;

		ResT* Query(const std::string& _assetPath) const;

		void Clear();


		ResourceMgr& operator=(ResourceMgr&&) = delete;
		ResourceMgr& operator=(const ResourceMgr&) = delete;
	};
}

#include <SDK/Assets/ResourceManager.inl>

#endif // GUARD
