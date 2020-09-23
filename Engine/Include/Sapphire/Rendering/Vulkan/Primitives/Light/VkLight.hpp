// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_LIGHT_GUARD
#define SAPPHIRE_RENDERING_VK_LIGHT_GUARD

#include <Rendering/Framework/Primitives/Light/ILight.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	class VkLight : public ILight
	{
	protected:
		VkUniformBuffer mUniformBuffer;

	public:
		void Create(const IRenderInstance& _instance, const LightInfos& _infos) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		VkDescriptorBufferInfo CreateDescriptorBufferInfo() const noexcept;
	};

#endif
}

#endif // GUARD
