// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_SHADER_GUARD
#define SAPPHIRE_RENDERING_VK_SHADER_GUARD

#include <string>

#include <vulkan/vulkan.h>

#include <Rendering/Config.hpp>
#include <Rendering/Framework/Primitives/Pipeline/IShader.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkShader : public IShader
	{
		VkShaderModule mHandle = VK_NULL_HANDLE;

	public:
		void SA_ENGINE_API Create(const IRenderInstance& _instance, ShaderType _type, const std::wstring& _fileName) override final;
		void SA_ENGINE_API Destroy(const IRenderInstance& _instance) override final;

		operator VkShaderModule() const;
	};
}

#endif

#endif // GUARD
