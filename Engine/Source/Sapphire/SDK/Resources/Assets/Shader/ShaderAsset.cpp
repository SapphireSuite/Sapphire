// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Shader/ShaderAsset.hpp>

#include <Core/Algorithms/Move.hpp>
#include <Core/Types/Variadics/SizeOf.hpp>

#include <SDK/Resources/Assets/AssetManager.hpp>

namespace Sa
{
#if SA_DEBUG

	// Supported extensions.
	constexpr const char* extensions[]{ "vert", "frag", "spv" }; // TODO: Remove spv

#endif

	ShaderAsset::ShaderAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::Shader)
	{
	}

	ShaderAsset::ShaderAsset(IResourceMgrBase& _manager, ShaderCreateInfos&& _createInfos) noexcept : IAsset(_manager, AssetType::Shader, Move(_createInfos))
	{
		mData = _createInfos.data;
		mSize = _createInfos.size;

		_createInfos.data = nullptr;
		_createInfos.size = 0u;
	}

	ShaderAsset::ShaderAsset(ShaderAsset&& _other) noexcept : IAsset(Move(_other))
	{
		mData = _other.mData;
		mSize = _other.mSize;

		_other.UnLoad(false);
	}

	ShaderAsset::~ShaderAsset()
	{
		UnLoad_Internal(true);
	}

	IShader* ShaderAsset::GetResource() const
	{
		return mManager.As<ResourceMgr<IShader, ShaderAsset>>().Query(mFilePath);
	}

	bool ShaderAsset::IsValid() const noexcept
	{
		return mSize && mData;
	}


	bool ShaderAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		// Header.
		if (!(_hStream >> mSize))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		mData = new char[mSize] {};
		_fStream.read(mData, mSize);

		return true;
	}

	void ShaderAsset::UnLoad_Internal(bool _bFreeResources)
	{
		if (mData && _bFreeResources)
			delete mData;

		mData = nullptr;
		mSize = 0u;
	}


	void ShaderAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(mData, Nullptr, SDK_Asset, L"Save nullptr texture asset!");

		// Header.
		_fStream << mSize << '\n';

		_fStream.write(mData, mSize);
	}

	void ShaderAsset::Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		IAsset::Import_Internal(_resourcePath, _importInfos);

		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			WrongExtension, SDK_Import, L"Shader file extension not supported yet!");

		// TODO: Compile shader.

		mFilePath = _importInfos.As<ShaderImportInfos>().outFilePath;

		std::ifstream file(_resourcePath, std::ios::binary | std::ios::ate);
		SA_ASSERT(file.is_open(), InvalidParam, Rendering, L"failed to open shader file!");

		mSize = static_cast<uint32>(file.tellg());
		mData = new char[mSize] {};

		file.seekg(0);
		file.read(mData, mSize);

		file.close();
	}

	ShaderAsset ShaderAsset::Import(IResourceMgrBase& _manager, const std::string& _resourcePath, const ShaderImportInfos& _importInfos)
	{
		ShaderAsset asset(_manager);

		asset.Import_Internal(_resourcePath, _importInfos);

		return asset;
	}


	IShader* ShaderAsset::Create(const IRenderInstance& _instance) const
	{
		return IShader::CreateInstance(_instance, mData, mSize);
	}

	ShaderAsset& ShaderAsset::operator=(ShaderAsset&& _rhs)
	{
		mData = _rhs.mData;
		mSize = _rhs.mSize;

		_rhs.UnLoad(false);

		return *this;
	}
}