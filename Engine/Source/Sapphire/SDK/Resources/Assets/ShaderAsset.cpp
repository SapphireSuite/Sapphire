// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/ShaderAsset.hpp>

#include <Core/Types/Variadics/SizeOf.hpp>

namespace Sa
{
#if SA_DEBUG

	// Supported extensions.
	constexpr const char* extensions[]{ "vert", "frag", "spv" }; // TODO: Remove spv

#endif

	ShaderAsset::ShaderAsset() noexcept : IAsset(AssetType::Shader)
	{
	}

	ShaderAsset::ShaderAsset(char* _data, uint32 _size) noexcept :
		IAsset(AssetType::Shader),
		mData{ _data },
		mSize{ _size }
	{
	}

	ShaderAsset::~ShaderAsset()
	{
		UnLoad_Internal(true);
	}

	void ShaderAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(mData, Nullptr, SDK_Asset, L"Save nullptr texture asset!");

		// Header.
		_fStream << mSize << '\n';

		_fStream.write(mData, mSize);
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

	IShader* ShaderAsset::Create(const IRenderInstance& _instance)
	{
		return IShader::CreateInstance(_instance, mData, mSize);
	}

	ShaderAsset ShaderAsset::Import(const std::string& _resourcePath)
	{
		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			InvalidParam, SDK_Import, L"Shader file extension not supported yet!");

		// TODO: Compile shader.

		std::ifstream file(_resourcePath, std::ios::binary | std::ios::ate);
		SA_ASSERT(file.is_open(), InvalidParam, Rendering, L"failed to open shader file!");

		uint32 fileSize = static_cast<uint32>(file.tellg());
		char* data = new char[fileSize] {};

		file.seekg(0);
		file.read(data, fileSize);

		file.close();

		return ShaderAsset(data, fileSize);
	}

	ShaderAsset& ShaderAsset::operator=(ShaderAsset&& _rhs)
	{
		mData = _rhs.mData;
		mSize = _rhs.mSize;

		_rhs.UnLoad(false);

		return *this;
	}
}