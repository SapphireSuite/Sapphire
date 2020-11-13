// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Shader/ShaderAsset.hpp>

#include <filesystem>
#include <fstream>

#include <Core/Algorithms/SizeOf.hpp>

namespace Sa
{
	constexpr const char tempDirectory[] = "Temp/Shaders/";


	ShaderAsset::ShaderAsset() noexcept : IAsset(AssetType::Shader)
	{
	}


	const RawShader& ShaderAsset::GetRawData() const noexcept
	{
		return mRawData;
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

	bool ShaderAsset::ShouldCompileShader(const std::string& _resourcePath, const std::string& _assetPath) noexcept
	{
		struct stat assetStat;
		struct stat resourceStat;

		return stat(_assetPath.c_str(), &assetStat) != 0 ||
			stat(_resourcePath.c_str(), &resourceStat) != 0 ||
			assetStat.st_mtime < resourceStat.st_mtime;
	}

	void ShaderAsset::CompileShader(const std::string& _resourcePath, const std::string& _tempPath)
	{
		SA_LOG("Compiling shader...", Infos, SDK_Asset);

		std::filesystem::create_directories(tempDirectory);

		std::string command = "..\\..\\Engine\\ThirdParty\\Tools\\GLSL\\glslc.exe " + _resourcePath + " -o " + _tempPath;

		int32 result = system(command.c_str());

		SA_ASSERT(result == 0, CompilationFailed, SDK_Assert, L"Shader compilation failed!");

		SA_LOG("Compilation Success", Infos, SDK_Asset);
	}


	bool ShaderAsset::IsValid() const noexcept
	{
		return !mRawData.data.empty() && !mResourcePath.empty();
	}

	bool ShaderAsset::Load_Internal(const std::string& _filePath, std::fstream& _fStream)
	{
		// Resource path.
		uint32 resourcePathSize = 0u;
		_fStream.read(reinterpret_cast<char*>(&resourcePathSize), sizeof(uint32));

		SA_ASSERT(resourcePathSize != 0u, InvalidParam, SDK_Asset, L"Asset load failure!");

		mResourcePath.resize(resourcePathSize);
		_fStream.read(mResourcePath.data(), resourcePathSize);


		if (!ShouldCompileShader(mResourcePath, _filePath)) // Read saved shader.
		{
			// Data.
			uint32 dataSize = 0u;
			_fStream.read(reinterpret_cast<char*>(&dataSize), sizeof(uint32));

			SA_ASSERT(dataSize != 0u, InvalidParam, SDK_Asset, L"Asset load failure!");

			mRawData.data.resize(dataSize);
			_fStream.read(mRawData.data.data(), dataSize);
		}
		else if (!Import(mResourcePath, ShaderImportInfos())) // Reimport shader.
		{
			// TODO: default import infos?
			SA_LOG("Re-Import compilation failed!", Error, SDK_Asset);
			return false;
		}

		return true;
	}

	void ShaderAsset::UnLoad_Internal()
	{
		mResourcePath.clear();
		mRawData.data.clear();
	}

	void ShaderAsset::Save_Internal(std::fstream& _fStream) const
	{
		// Resource path.
		const uint32 resourcePathSize = SizeOf(mResourcePath);
		_fStream.write(reinterpret_cast<const char*>(&resourcePathSize), sizeof(uint32));

		_fStream.write(mResourcePath.data(), resourcePathSize);


		// Data.
		const uint32 dataSize = SizeOf(mRawData.data);
		_fStream.write(reinterpret_cast<const char*>(&dataSize), sizeof(uint32));
		
		_fStream.write(mRawData.data.data(), dataSize);
	}

	bool ShaderAsset::Import(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		(void)_importInfos;

		std::string tempPath = GenerateTempPath(_resourcePath);
		CompileShader(_resourcePath, tempPath);

		mResourcePath = _resourcePath;

		std::ifstream file(tempPath, std::ios::binary | std::ios::ate);
		SA_ASSERT(file.is_open(), InvalidParam, Rendering, L"failed to open shader file!");

		uint32 size = static_cast<uint32>(file.tellg());
		mRawData.data.resize(size);

		file.seekg(0);
		file.read(mRawData.data.data(), size);

		file.close();

		// Delete temp file.
		std::filesystem::remove(tempPath);

		return true;
	}
}