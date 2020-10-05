// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/MaterialAsset.hpp>

#include <Core/Algorithms/Move.hpp>

#include <Core/Types/Variadics/SizeOf.hpp>

#include <Rendering/Framework/System/IRenderSurface.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

namespace Sa
{
#if SA_DEBUG

	// Supported extensions.
	constexpr const char* extensions[]{ "mtl" };

#endif

	MaterialAsset::MaterialAsset() noexcept : IAsset(AssetType::Material)
	{
	}

	MaterialAsset::~MaterialAsset()
	{
		UnLoad_Internal(true);
	}

	void MaterialAsset::Save_Internal(std::fstream& _fStream) const
	{
		// Header.
		_fStream << '\n';

		// Shaders.
		_fStream << vertexShaderPath << ' ' << fragmentShaderPath << '\n';

		// Textures.
		_fStream << texturePaths.size() << ' ';

		for (auto it = texturePaths.begin(); it != texturePaths.end(); ++it)
			_fStream << *it << ' ';

		_fStream << '\n';


		// Data.
		uint32 size = sizeof(MaterialAsset) - offsetof(MaterialAsset, uniformBufferSize);
		_fStream.write(reinterpret_cast<const char*>(&uniformBufferSize), size);
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

			if (!(stream >> vertexShaderPath >> fragmentShaderPath))
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

			texturePaths.resize(size);

			for (auto it = texturePaths.begin(); it != texturePaths.end(); ++it)
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
			uint32 size = sizeof(MaterialAsset) - offsetof(MaterialAsset, uniformBufferSize);
			_fStream.read(reinterpret_cast<char*>(&uniformBufferSize), size);
		}

		return true;
	}

	void MaterialAsset::UnLoad_Internal(bool _bFreeResources)
	{
		vertexShaderPath.clear();
		fragmentShaderPath.clear();

		texturePaths.clear();
	}


	IRenderMaterial* MaterialAsset::Create(const IRenderInstance& _instance)
	{
		// TODO LATER.
		return nullptr;
	}

	std::vector<MaterialAsset> MaterialAsset::Import(const std::string& _resourcePath)
	{
		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			InvalidParam, SDK_Import, L"Shader file extension not supported yet!");

		// Todo: parse mtl...

		return std::vector<MaterialAsset>();
	}


	MaterialAsset& MaterialAsset::operator=(MaterialAsset&& _rhs)
	{
		vertexShaderPath = Move(_rhs.vertexShaderPath);
		fragmentShaderPath = Move(_rhs.fragmentShaderPath);

		texturePaths = Move(_rhs.texturePaths);

		uniformBufferSize = _rhs.uniformBufferSize;

		alphaModel = _rhs.alphaModel;
		polygonMode = _rhs.polygonMode;
		cullingMode = _rhs.cullingMode;
		frontFaceMode = _rhs.frontFaceMode;
		illumModel = _rhs.illumModel;

		return *this;
	}
}