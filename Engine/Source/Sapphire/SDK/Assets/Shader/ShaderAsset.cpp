// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Shader/ShaderAsset.hpp>

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

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	constexpr const char tempDirectory[] = "Temp/Shaders/";

	ShaderAsset::ShaderAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::Shader)
	{
	}

	ShaderAsset::ShaderAsset(IResourceMgrBase& _manager, RawT&& _raw) noexcept :
		IAsset(_manager, AssetType::Shader),
		mRawData{ _raw }
	{
	}

	ShaderAsset::ShaderAsset(ShaderAsset&& _other) noexcept :
		IAsset(Move(_other)),
		mRawData{ Move(_other.mRawData) }
	{
		_other.UnLoad(false);
	}

	ShaderAsset::~ShaderAsset()
	{
		UnLoad_Internal(true);
	}

	IShader* ShaderAsset::GetResource() const
	{
		IShader* result = nullptr;
		auto& manager = mManager.As<ResourceMgr<ShaderAsset>>();

		result = manager.Query(mFilePath);

		if (result)
			return result;

		return manager.Load(*this);
	}

	bool ShaderAsset::IsValid() const noexcept
	{
		return mRawData.size && mRawData.data;
	}

	bool ShaderAsset::PostLoadOperation(const std::string& _filePath)
	{
		if (!IAsset::PostLoadOperation(_filePath))
			return false;

		if (mHasRecompiled)
			Save();

		return true;
	}

	bool ShaderAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		// Header.
		if (!(_hStream >> mResourcePath >> mRawData.size))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		if (ShouldCompileShader(mResourcePath, mFilePath)) // Reimport shader.
		{
			mHasRecompiled = true;
			Import(mResourcePath);
		}
		else
		{
			mRawData.data = new char[mRawData.size] {};
			_fStream.read(mRawData.data, mRawData.size);
		}

		return true;
	}

	void ShaderAsset::UnLoad_Internal(bool _bFreeResources)
	{
		if (mRawData.data && _bFreeResources)
			delete mRawData.data;

		mRawData.data = nullptr;
		mRawData.size = 0u;
	}


	void ShaderAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(mRawData.data, Nullptr, SDK_Asset, L"Save nullptr texture asset!");

		// Header.
		_fStream << mResourcePath.c_str() << ' ' << mRawData.size << '\n';

		_fStream.write(mRawData.data, mRawData.size);
	}

	void ShaderAsset::Import(const std::string& _resourcePath)
	{
		std::string tempPath = GenerateTempPath(_resourcePath);
		CompileShader(_resourcePath, tempPath);

		mResourcePath = _resourcePath;

		std::ifstream file(tempPath, std::ios::binary | std::ios::ate);
		SA_ASSERT(file.is_open(), InvalidParam, Rendering, L"failed to open shader file!");

		mRawData.size = static_cast<uint32>(file.tellg());
		mRawData.data = new char[mRawData.size] {};

		file.seekg(0);
		file.read(mRawData.data, mRawData.size);

		file.close();
		
		// Delete temp file.
		std::filesystem::path systemTempPath(tempPath);
		systemTempPath.remove_filename();
	}


	IShader* ShaderAsset::Create(const IRenderInstance& _instance) const
	{
		return IShader::CreateInstance(_instance, mRawData);
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

	bool ShaderAsset::ShouldCompileShader(const std::string& _resourcePath, const std::string& _assetPath)
	{
		struct stat assetStat;
		struct stat resourceStat;

		return stat(_assetPath.c_str(), &assetStat) != 0 ||
			stat(_resourcePath.c_str(), &resourceStat) != 0 ||
			assetStat.st_mtime < resourceStat.st_mtime;
	}

	void ShaderAsset::CompileShader(const std::string& _resourcePath, const std::string& _tempPath)
	{
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
		mRawData = Move(_rhs.mRawData);

		_rhs.UnLoad(false);

		return *this;
	}
}