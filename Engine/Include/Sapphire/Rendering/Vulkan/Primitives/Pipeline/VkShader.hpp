// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SHADER_GUARD
#define SAPPHIRE_RENDERING_VK_SHADER_GUARD

#include <string>

#include <vulkan/vulkan.h>

#include <Rendering/Config.hpp>
#include <Rendering/Framework/Primitives/Pipeline/Shader.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;

	class VkShader : public Shader
	{
		VkShaderModule mHandle = VK_NULL_HANDLE;

	public:
		//void Create(const VkDevice& _device, const std::string& filename);
		//void Destroy(const VkDevice& _device);

		operator VkShaderModule() const;
	};
}

#endif

#endif // GUARD
