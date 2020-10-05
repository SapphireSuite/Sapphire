// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_IASSET_GUARD
#define SAPPHIRE_SDK_IASSET_GUARD

#include <string>
#include <fstream>
#include <sstream>

#include <Core/Types/IInterface.hpp>

#include <SDK/Config.hpp>

namespace Sa
{
	enum class AssetType
	{
		Unknown = 0,

		Texture,

		Shader,

		Material,

		Mesh,

		Model
	};

	class IAsset : public IInterface
	{
	protected:
		std::string mFilePath;

		IAsset(AssetType _assetType) noexcept;

		virtual void Save_Internal(std::fstream& _fStream) const = 0;

		virtual bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) = 0;
		virtual void UnLoad_Internal(bool _bFreeResources) = 0;

		static std::string GetResourceExtension(const std::string& _resourcePath);

#if SA_DEBUG
		static bool CheckExtensionSupport(const std::string& _resourcePath, const char* const* _extensions, uint32 _extensionSize);
#endif

	public:
		const AssetType assetType = AssetType::Unknown;


		IAsset(IAsset&&) = delete;
		IAsset(const IAsset&) = delete;

		virtual ~IAsset() = default;


		const std::string& GetFilePath() const;

		SA_ENGINE_API void Save(const std::string& _filePath = "");

		SA_ENGINE_API bool Load(const std::string& _filePath);
		SA_ENGINE_API void UnLoad(bool _bFreeResources = true);


		SA_ENGINE_API IAsset& operator=(IAsset&& _rhs);
		IAsset& operator=(const IAsset&) = delete;
	};
}

#endif // GUARD
