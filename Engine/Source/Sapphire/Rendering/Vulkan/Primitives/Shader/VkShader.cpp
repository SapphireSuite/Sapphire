// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	void Shader::Create(const IRenderInstance& _instance, const RawShader& _rawShader)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		VkShaderModuleCreateInfo shaderModuleCreateInfo{};
		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.pNext = nullptr;
		shaderModuleCreateInfo.flags = 0u;
		shaderModuleCreateInfo.codeSize = _rawShader.data.size();
		shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32*>(_rawShader.data.data());

		SA_VK_ASSERT(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create shader module!");
	}

	void Shader::Destroy(const IRenderInstance& _instance)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, InvalidParam, Rendering, L"Destroy a null Shader!");

		const Device& device = _instance.As<RenderInstance>().device;

		vkDestroyShaderModule(device, mHandle, nullptr);

		mHandle = VK_NULL_HANDLE;
	}

	Shader::operator VkShaderModule() const
	{
		return mHandle;
	}
}

#endif