// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Material/MaterialAsset.hpp>

#include <tiny_obj_loader.h>

#include <Core/Algorithms/Move.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Framework/System/IRenderSurface.hpp>
#include <Rendering/Framework/Primitives/Material/RenderMaterialCreateInfos.hpp>

#include <SDK/Resources/Assets/AssetManager.hpp>

namespace Sa
{
#if SA_DEBUG

	// Supported extensions.
	constexpr const char* extensions[]{ "mtl" };

#endif

	MaterialAsset::MaterialAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::RenderMaterial)
	{
	}

	MaterialAsset::MaterialAsset(MaterialAsset&& _other) noexcept : IAsset(Move(_other))
	{
		infos = Move(_other.infos);
		
		_other.UnLoad(false);
	}

	MaterialAsset::MaterialAsset(IResourceMgrBase& _manager, MaterialCreateInfos&& _infos) noexcept :
		IAsset(_manager, AssetType::RenderMaterial, Move(_infos))
	{
		infos = Move(_infos);
	}

	MaterialAsset::~MaterialAsset()
	{
		UnLoad_Internal(true);
	}


	IRenderMaterial* MaterialAsset::GetResource() const
	{
		return mManager.As<ResourceMgr<IRenderMaterial, MaterialAsset>>().Query(mFilePath);
	}

	bool MaterialAsset::IsValid() const noexcept
	{
		return !infos.vertexShaderPath.empty() && !infos.fragmentShaderPath.empty();
	}

	std::vector<AssetPathDependency> MaterialAsset::GetPathDependencies() const noexcept
	{
		std::vector<AssetPathDependency> result = IAsset::GetPathDependencies();

		result.reserve(infos.texturePaths.size() + 2);

		result.emplace_back(AssetPathDependency{ infos.vertexShaderPath, AssetType::Shader });
		result.emplace_back(AssetPathDependency{ infos.fragmentShaderPath, AssetType::Shader });

		for (auto it = infos.texturePaths.begin(); it != infos.texturePaths.end(); ++it)
			result.emplace_back(AssetPathDependency{ *it, AssetType::Texture });

		return result;
	}

	bool MaterialAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		std::string line;
		
		// Shaders.
		{
			if (!std::getline(_fStream, line))
			{
				SA_LOG("Can't parse shader paths", Warning, SDK_Asset);
				return false;
			}

			std::istringstream stream(line);

			if (!(stream >> infos.vertexShaderPath >> infos.fragmentShaderPath))
			{
				SA_LOG("Can't parse shader paths!", Warning, SDK_Asset);
				return false;
			}
		}


		// Textures.
		{
			if (!std::getline(_fStream, line))
			{
				SA_LOG("Can't parse textures paths", Warning, SDK_Asset);
				return false;
			}

			uint32 size = 0u;
			std::istringstream stream(line);

			if (!(stream >> size))
			{
				SA_LOG("Can't parse texture path size!", Warning, SDK_Asset);
				return false;
			}

			infos.texturePaths.resize(size);

			for (auto it = infos.texturePaths.begin(); it != infos.texturePaths.end(); ++it)
			{
				if (!(stream >> *it))
				{
					SA_LOG("Can't parse texture path!", Warning, SDK_Asset);
					return false;
				}
			}
		}

		// Data
		{
			_fStream.read(reinterpret_cast<char*>(&infos.bDynamicViewport), sizeof(bool));

			uint32 size = sizeof(MaterialCreateInfos) - offsetof(MaterialCreateInfos, matConstants);
			_fStream.read(reinterpret_cast<char*>(&infos.matConstants), size);
		}

		return true;
	}

	void MaterialAsset::UnLoad_Internal(bool _bFreeResources)
	{
		infos.vertexShaderPath.clear();
		infos.fragmentShaderPath.clear();

		infos.texturePaths.clear();
	}


	void MaterialAsset::Save_Internal(std::fstream& _fStream) const
	{
		// Header.
		_fStream << '\n';

		// Shaders.
		_fStream << infos.vertexShaderPath << ' ' << infos.fragmentShaderPath << '\n';

		// Textures.
		_fStream << infos.texturePaths.size() << ' ';

		for (auto it = infos.texturePaths.begin(); it != infos.texturePaths.end(); ++it)
			_fStream << *it << ' ';

		_fStream << '\n';


		// Data.
		_fStream.write(reinterpret_cast<const char*>(&infos.bDynamicViewport), sizeof(bool));

		uint32 size = sizeof(MaterialCreateInfos) - offsetof(MaterialCreateInfos, matConstants);
		_fStream.write(reinterpret_cast<const char*>(&infos.matConstants), size);
	}

	void MaterialAsset::Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		IAsset::Import_Internal(_resourcePath, _importInfos);
	}

	IRenderMaterial* MaterialAsset::Create(const IRenderInstance& _instance) const
	{
		IRenderMaterial* result = IRenderMaterial::CreateInstance();

		AssetManager& assetMgr = mManager.As<ResourceMgr<IRenderMaterial, MaterialAsset>>().GetAssetMgr();

		// Shaders.
		const IShader* vertShader = assetMgr.shaderMgr.Load(infos.vertexShaderPath);
		SA_ASSERT(vertShader, Nullptr, SDK_Asset, L"Shader asset nulltpr! Path invalid");

		const IShader* fragShader = assetMgr.shaderMgr.Load(infos.fragmentShaderPath);
		SA_ASSERT(fragShader, Nullptr, SDK_Asset, L"Shader asset nulltpr! Path invalid");


		// Textures
		std::vector<const ITexture*> textures(infos.texturePaths.size());

		for (uint32 i = 0u; i < SizeOf(infos.texturePaths); ++i)
		{
			const ITexture* texture = assetMgr.textureMgr.Load(infos.texturePaths[i]);
			SA_ASSERT(texture, Nullptr, SDK_Asset, L"Texture asset nulltpr! Path invalid");

			textures[i] = texture;
		}

		const RenderMaterialCreateInfos matCreateInfos
		{
			infos.renderPasses,

			infos.cameras,
			infos.bDynamicViewport,

			vertShader,
			fragShader,

			textures,

			infos.matConstants,
			infos.renderInfos
		};

		result->Create(_instance, matCreateInfos);

		return result;
	}

	std::vector<MaterialAsset> MaterialAsset::Import(IResourceMgrBase& _manager, const std::string& _resourcePath, const MaterialImportInfos& _importInfos)
	{
		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			WrongExtension, SDK_Import, L"Material file extension not supported yet!");

		std::vector<MaterialAsset> result;
		std::string extension = GetResourceExtension(_resourcePath);

		if (extension == "mtl")
			result = ImportMTL(_manager, _resourcePath);
		else
			SA_ASSERT(false, WrongExtension, SDK_Import, L"Material file extension not supported yet!");


		for (uint32 i = 0u; i < SizeOf(result); ++i)
		{
			result[i].mFilePath = _importInfos.outFilePaths[i];
			result[i].Import_Internal(_resourcePath, _importInfos);
		}

		return result;
	}

	std::vector<MaterialAsset> MaterialAsset::ImportMTL(IResourceMgrBase& _manager, const std::string& _resourcePath)
	{
		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			InvalidParam, SDK_Import, L"Shader file extension not supported yet!");

		std::map<std::string, int> matMap;
		std::vector<tinyobj::material_t> materials;
		std::ifstream stream(_resourcePath);
		std::string warn; std::string error;

		tinyobj::LoadMtl(&matMap, &materials, &stream, &warn, &error);

		std::vector<MaterialAsset> result;
		result.reserve(materials.size());

		for (uint32 i = 0u; i < SizeOf(materials); ++i)
		{
			MaterialAsset& resMat = result.emplace_back(_manager);

			resMat.infos.matConstants.ambient = Vec3f(materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
			resMat.infos.matConstants.diffuse = Vec3f(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
			resMat.infos.matConstants.specular = Vec3f(materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
			resMat.infos.matConstants.emissive = Vec3f(materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);

			resMat.infos.matConstants.alpha = materials[i].dissolve;

			resMat.infos.matConstants.shininess = materials[i].shininess;

			resMat.infos.matConstants.refractIndex = materials[i].ior;
		}

		return result;
	}


	MaterialAsset& MaterialAsset::operator=(MaterialAsset&& _rhs)
	{
		infos = Move(_rhs.infos);

		_rhs.UnLoad(false);

		return *this;
	}
}