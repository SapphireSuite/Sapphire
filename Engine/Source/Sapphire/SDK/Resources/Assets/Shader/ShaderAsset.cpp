// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Shader/ShaderAsset.hpp>

#include <filesystem>

#include <iostream>
#if SA_WIN
	#include <sys/types.h>
	#include <sys/stat.h>
#elif SA_UNIX
	#include <unistd.h>
#endif

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <SDK/Resources/Assets/AssetManager.hpp>

namespace Sa
{
#if SA_DEBUG

	constexpr const char* extensions[]{ "vert", "frag" }; // TODO: Remove spv

#endif

	constexpr const char tempDirectory[] = "Temp/Shaders/";

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
		if (!(_hStream >> mResourcePath >> mSize))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		std::string tempPath = GenerateTempPath(mResourcePath);
		CompileShader(mResourcePath, tempPath);

		if (mHasRecompiled)
		{
			std::fstream newShaderFile(tempPath, std::ios::binary | std::ios_base::in);

			mSize = static_cast<uint32>(newShaderFile.tellg());
			mData = new char[mSize] {};

			newShaderFile.seekg(0);
			newShaderFile.read(mData, mSize);

			newShaderFile.close();
		}
		else
		{
			mData = new char[mSize] {};
			_fStream.read(mData, mSize);
		}

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
		_fStream << mResourcePath.c_str() << ' ' << mSize << '\n';

		_fStream.write(mData, mSize);
	}

	void ShaderAsset::Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		IAsset::Import_Internal(_resourcePath, _importInfos);

		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			WrongExtension, SDK_Import, L"Shader file extension not supported yet!");

		std::string compiledPath = GenerateTempPath(_resourcePath);
		CompileShader(_resourcePath, compiledPath);

		mResourcePath = _resourcePath;
		mFilePath = _importInfos.As<ShaderImportInfos>().outFilePath;

		std::ifstream file(compiledPath, std::ios::binary | std::ios::ate);
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

	std::string ShaderAsset::GenerateTempPath(const std::string& _resourcePath)
	{
		std::string tempPath;

		uint32 nameIndex = _resourcePath.find_last_of('/') + 1;
		SA_ASSERT(nameIndex != 0, InvalidParam, SDK_Asset, L"Invalid resource path!");

		uint32 extensionIndex = _resourcePath.find_last_of('.');
		SA_ASSERT(extensionIndex != uint32(-1), InvalidParam, SDK_Asset, L"Invalid resource path!");

		std::string extension = _resourcePath.substr(extensionIndex + 1);

		tempPath.reserve(SizeOf(tempDirectory) + extensionIndex - nameIndex + extension.size() + 5);

		tempPath = tempDirectory + _resourcePath.substr(nameIndex, extensionIndex - nameIndex) + '_' + extension + ".spv";

		return tempPath;
	}

	bool ShaderAsset::ShouldCompileShader(const std::string& _resourcePath, const std::string& _tempPath)
	{
		struct stat tempStat;
		struct stat resourceStat;

		return stat(_tempPath.c_str(), &tempStat) != 0 ||
			stat(_resourcePath.c_str(), &resourceStat) != 0 ||
			tempStat.st_mtime < resourceStat.st_mtime;
	}

	void ShaderAsset::CompileShader(const std::string& _resourcePath, const std::string& _tempPath)
	{
		if (!ShouldCompileShader(_resourcePath, _tempPath))
			return;

		mHasRecompiled = true;
		SA_LOG("Compiling shader...", Infos, SDK_Asset);
			
		std::filesystem::create_directories(tempDirectory);

		std::string command = "..\\..\\Engine\\ThirdParty\\Tools\\GLSL\\glslc.exe " + _resourcePath + " -o " + _tempPath;

		int32 result = system(command.c_str());

		SA_ASSERT(result == 0, CompilationFailed, SDK_Assert, L"Shader compilation failed!");

		SA_LOG("Compilation Success", Infos, SDK_Asset);
	}


	ShaderAsset& ShaderAsset::operator=(ShaderAsset&& _rhs)
	{
		mData = _rhs.mData;
		mSize = _rhs.mSize;

		_rhs.UnLoad(false);

		return *this;
	}
}