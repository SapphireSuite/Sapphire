// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Importer/AssetImporter.hpp>

#include <Collections/Debug>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <SDK/Assets/AssetManager.hpp>
#include <SDK/Wrappers/StbiWrapper.hpp>
#include <SDK/Wrappers/TinyOBJWrapper.hpp>
#include <SDK/Wrappers/TinyGLTFWrapper.hpp>

namespace Sa
{
	// Supported extensions.
	constexpr const char* shaderExts[]{ "vert", "frag" };
	constexpr const char* textureExts[]{ "png", "jpg", "jpeg", "bmp", "tga" };

	AssetImporter::AssetImporter(AssetManager& _manager) noexcept : mManager{ _manager }
	{
	}


	std::string AssetImporter::GetResourceExtension(const std::string& _resourcePath)
	{
		uint32 extIndex = _resourcePath.find_last_of('.');

		SA_ASSERT(extIndex != uint32(-1), InvalidParam, SDK_Import, L"File path invalid extension!");
		SA_ASSERT(extIndex + 1 < _resourcePath.size(), InvalidParam, SDK_Import, L"File path invalid extension!");

		return _resourcePath.substr(extIndex + 1);
	}

	bool AssetImporter::ContainExt(const std::string& ext, const char* const* _extensions, uint32 _extensionSize)
	{
		for (uint32 i = 0; i < _extensionSize; ++i)
		{
			if (ext == _extensions[i])
				return true;
		}

		return false;
	}

	IAssetImportResult AssetImporter::Import(const std::string& _resourcePath)
	{
		IAssetImportResult result;
		std::string ext = GetResourceExtension(_resourcePath);

		if (TryImportModel(_resourcePath, ext, result))
			return result;

		if (TryImportTexture(_resourcePath, ext, result))
			return result;

		if (TryImportShader(_resourcePath, ext, result))
			return result;

		if (TryImportRenderMaterial(_resourcePath, ext, result))
			return result;


		SA_ASSERT(false, WrongExtension, SDK_Asset, L"Resource type not supported yet!");

		return IAssetImportResult();
	}

	IAssetImportResult AssetImporter::ImportCubemap(const CubemapAssetImportInfos& _importInfos)
	{
		IAssetImportResult result;
		std::string ext = GetResourceExtension(_importInfos.pathes[0]);

		if (!ContainExt(ext, textureExts, SizeOf(textureExts)))
			return result;

		StbiWrapper::ImportCubemap(_importInfos, mManager, result);

		return result;
	}


	bool AssetImporter::TryImportModel(const std::string& _resourcePath, const std::string& _extension, IAssetImportResult& _result)
	{
		if (_extension == "obj")
			return TinyOBJWrapper::ImportOBJ(_resourcePath, mManager, _result);
		else if (_extension == "gltf")
			return TinyGLTFWrapper::ImportGLTF(_resourcePath, mManager, _result);
		else if (_extension == "glb")
			return TinyGLTFWrapper::ImportGLB(_resourcePath, mManager, _result);

		return false;
	}


	bool AssetImporter::TryImportTexture(const std::string& _resourcePath, const std::string& _extension, IAssetImportResult& _result)
	{
		if (!ContainExt(_extension, textureExts, SizeOf(textureExts)))
			return false;

		return StbiWrapper::Import(_resourcePath, mManager, _result);
	}


	bool AssetImporter::TryImportShader(const std::string& _resourcePath, const std::string& _extension, IAssetImportResult& _result)
	{
		if (!ContainExt(_extension, shaderExts, SizeOf(shaderExts)))
			return false;

		ShaderAsset* asset = new ShaderAsset(mManager.shaderMgr);

		asset->Import(_resourcePath);

		_result.push_back(asset);

		return true;
	}


	bool AssetImporter::TryImportRenderMaterial(const std::string& _resourcePath, const std::string& _extension,
		IAssetImportResult& _result)
	{
		if (_extension == "mtl")
			return TinyOBJWrapper::ImportMTL(_resourcePath, mManager, _result);

		return false;
	}
}