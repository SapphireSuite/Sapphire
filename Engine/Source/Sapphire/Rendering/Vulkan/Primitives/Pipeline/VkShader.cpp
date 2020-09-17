// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <fstream>

#include <Rendering/Vulkan/Primitives/Pipeline/VkShader.hpp>

#include <Rendering/Vulkan/VkRenderInstance.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	std::vector<char> ReadFile(const std::wstring& filename)
	{
		std::ifstream file(filename, std::ios::binary);

		SA_ASSERT(file.is_open(), InvalidParam, Rendering, L"failed to open shader file!");

		// Set cursor at end of file. file.open(filename, std::ios::ate | std::ios::binary) doesn't work.
		file.seekg(0, std::ios::end);

		uint32 fileSize = static_cast<uint32>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	void VkShader::Create(const IRenderInstance& _instance, ShaderType _type, const std::wstring& _fileName)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		auto shaderCode = ReadFile(_fileName);

		VkShaderModuleCreateInfo shaderModuleCreateInfo
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,					// sType.
			nullptr,														// pNext.
			0,																// flags.
			shaderCode.size(),												// codeSize.
			reinterpret_cast<const uint32*>(shaderCode.data())				// pCode.
		};

		SA_VK_ASSERT(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create shader module!");

		mShaderType = _type;
	}
	void VkShader::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		vkDestroyShaderModule(device, mHandle, nullptr);

		mHandle = VK_NULL_HANDLE;
		mShaderType = ShaderType::Unknown;
	}

	VkShader::operator VkShaderModule() const
	{
		return mHandle;
	}
}

#endif