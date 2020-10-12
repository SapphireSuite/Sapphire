// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/RenderMaterial/RenderMaterialAsset.hpp>

#include <tiny_obj_loader.h>

#include <Core/Algorithms/Move.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Framework/System/IRenderSurface.hpp>
#include <Rendering/Framework/Primitives/Material/RenderMaterialCreateInfos.hpp>

// TODO: REMOVE LATER.
#include <Rendering/Framework/System/RenderPass/IRenderPass.hpp>
#include <Rendering/Framework/Primitives/Camera/Camera.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	RenderMaterialAsset::RenderMaterialAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::RenderMaterial)
	{
	}

	RenderMaterialAsset::RenderMaterialAsset(RenderMaterialAsset&& _other) noexcept :
		IAsset(Move(_other)),
		infos{ Move(_other.infos) }
	{
		_other.UnLoad(false);
	}

	RenderMaterialAsset::RenderMaterialAsset(IResourceMgrBase& _manager, RawT&& _raw) noexcept :
		IAsset(_manager, AssetType::RenderMaterial),
		infos{ Move(_raw) }
	{
	}

	RenderMaterialAsset::~RenderMaterialAsset()
	{
		UnLoad_Internal(true);
	}


	IRenderMaterial* RenderMaterialAsset::GetResource() const
	{
		IRenderMaterial* result = nullptr;
		auto& manager = mManager.As<ResourceMgr<RenderMaterialAsset>>();

		result = manager.Query(mFilePath);

		if (result)
			return result;

		return manager.Load(*this);
	}

	bool RenderMaterialAsset::IsValid() const noexcept
	{
		return !infos.vertexShaderPath.empty() && !infos.fragmentShaderPath.empty();
	}

	std::vector<AssetPathDependency> RenderMaterialAsset::GetPathDependencies() const noexcept
	{
		std::vector<AssetPathDependency> result = IAsset::GetPathDependencies();

		result.reserve(infos.texturePaths.size() + 2);

		result.emplace_back(AssetPathDependency{ infos.vertexShaderPath, AssetType::Shader });
		result.emplace_back(AssetPathDependency{ infos.fragmentShaderPath, AssetType::Shader });

		for (auto it = infos.texturePaths.begin(); it != infos.texturePaths.end(); ++it)
			result.emplace_back(AssetPathDependency{ *it, AssetType::Texture });

		return result;
	}

	bool RenderMaterialAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
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

			uint32 size = sizeof(RenderMaterialCreateInfos) - offsetof(RenderMaterialCreateInfos, matConstants);
			_fStream.read(reinterpret_cast<char*>(&infos.matConstants), size);
		}

		return true;
	}

	void RenderMaterialAsset::UnLoad_Internal(bool _bFreeResources)
	{
		infos.vertexShaderPath.clear();
		infos.fragmentShaderPath.clear();

		infos.texturePaths.clear();
	}


	void RenderMaterialAsset::Save_Internal(std::fstream& _fStream) const
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

		uint32 size = sizeof(RenderMaterialCreateInfos) - offsetof(RenderMaterialCreateInfos, matConstants);
		_fStream.write(reinterpret_cast<const char*>(&infos.matConstants), size);
	}

	IRenderMaterial* RenderMaterialAsset::Create(const IRenderInstance& _instance) const
	{
		IRenderMaterial* result = IRenderMaterial::CreateInstance();

		AssetManager& assetMgr = mManager.As<ResourceMgr<RenderMaterialAsset>>().GetAssetMgr();

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
			*IRenderPass::mainRenderPass, // TODO: CLEAN LATER.

			{ Camera::mainCamera }, // TODO: CLEAN LATER.
			false, // TODO: CLEAN LATER.

			vertShader,
			fragShader,

			textures,

			infos.matConstants,
			infos.renderInfos
		};

		result->Create(_instance, matCreateInfos);

		return result;
	}


	RenderMaterialAsset& RenderMaterialAsset::operator=(RenderMaterialAsset&& _rhs)
	{
		infos = Move(_rhs.infos);

		_rhs.UnLoad(false);

		return *this;
	}
}