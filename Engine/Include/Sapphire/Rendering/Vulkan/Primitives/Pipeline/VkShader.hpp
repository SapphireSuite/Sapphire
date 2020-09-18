// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SHADER_GUARD
#define SAPPHIRE_RENDERING_VK_SHADER_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Pipeline/IShader.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkShader : public IShader
	{
		VkShaderModule mHandle = VK_NULL_HANDLE;

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create(const IRenderInstance& _instance, const std::wstring& _fileName) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy(const IRenderInstance& _instance) override final;

		operator VkShaderModule() const;
	};
}

#endif

#endif // GUARD
