// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <fstream>

#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkShader::Create(const IRenderInstance& _instance, const char* _code, uint32 _size)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		VkShaderModuleCreateInfo shaderModuleCreateInfo
		{
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,					// sType.
			nullptr,														// pNext.
			0,																// flags.
			_size,															// codeSize.
			reinterpret_cast<const uint32*>(_code)							// pCode.
		};

		SA_VK_ASSERT(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create shader module!");
	}

	void VkShader::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		vkDestroyShaderModule(device, mHandle, nullptr);

		mHandle = VK_NULL_HANDLE;
	}

	VkShader::operator VkShaderModule() const
	{
		return mHandle;
	}
}

#endif