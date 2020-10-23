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

	std::unique_ptr<IAsset> AssetImporter::Import(const std::string& _resourcePath)
	{
		SA_LOG("Import Asset", Infos, SDK_Asset);

		std::string ext = GetResourceExtension(_resourcePath);

		if (ModelAsset* asset = TryImportModel(_resourcePath, ext))
			return std::unique_ptr<IAsset>(asset);

		if (TextureAsset* asset = TryImportTexture(_resourcePath, ext))
			return std::unique_ptr<IAsset>(asset);

		if (ShaderAsset* asset = TryImportShader(_resourcePath, ext))
			return std::unique_ptr<IAsset>(asset);

		if (RenderMaterialAsset* asset = TryImportRenderMaterial(_resourcePath, ext))
			return std::unique_ptr<IAsset>(asset);


		SA_ASSERT(false, WrongExtension, SDK_Asset, L"Resource type not supported yet!");

		return nullptr;
	}

	CubemapAsset AssetImporter::Import(const CubemapAssetImportInfos& _importInfos)
	{
		CubemapAsset result = CubemapAsset(mManager);
		std::string ext = GetResourceExtension(_importInfos.pathes[0]);

		if (!ContainExt(ext, textureExts, SizeOf(textureExts)))
			return result;

		StbiWrapper::Import(_importInfos, result);

		return result;
	}


	ModelAsset* AssetImporter::TryImportModel(const std::string& _resourcePath, const std::string& _extension)
	{
		ModelAsset* result = new ModelAsset(mManager);

		bool importRes = false;

		if (_extension == "obj")
			importRes = TinyOBJWrapper::ImportOBJ(_resourcePath, *result);
		else if (_extension == "gltf")
			importRes = TinyGLTFWrapper::ImportGLTF(_resourcePath, *result);
		else if (_extension == "glb")
			importRes = TinyGLTFWrapper::ImportGLB(_resourcePath, *result);

		if (!importRes)
		{
			delete result;
			return nullptr;
		}

		return result;
	}


	TextureAsset* AssetImporter::TryImportTexture(const std::string& _resourcePath, const std::string& _extension)
	{
		if (!ContainExt(_extension, textureExts, SizeOf(textureExts)))
			return nullptr;

		TextureAsset* result = new TextureAsset(mManager);

		if (!StbiWrapper::Import(_resourcePath, *result))
		{
			delete result;
			return nullptr;
		}

		return result;
	}


	ShaderAsset* AssetImporter::TryImportShader(const std::string& _resourcePath, const std::string& _extension)
	{
		if (!ContainExt(_extension, shaderExts, SizeOf(shaderExts)))
			return nullptr;

		ShaderAsset* asset = new ShaderAsset(mManager);

		asset->Import(_resourcePath);

		return asset;
	}


	RenderMaterialAsset* AssetImporter::TryImportRenderMaterial(const std::string& _resourcePath, const std::string& _extension)
	{
		RenderMaterialAsset* result = new RenderMaterialAsset(mManager);

		bool importRes = false;

		// TODO: FIX.
		//if (_extension == "mtl")
		//	importRes = TinyOBJWrapper::ImportMTL(_resourcePath, *result);

		if (!importRes)
		{
			delete result;
			return nullptr;
		}

		return result;
	}
}