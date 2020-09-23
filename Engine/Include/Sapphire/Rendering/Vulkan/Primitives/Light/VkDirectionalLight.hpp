// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_DIRECTIONAL_LIGHT_GUARD
#define SAPPHIRE_RENDERING_VK_DIRECTIONAL_LIGHT_GUARD

#include <Rendering/Vulkan/Primitives/Light/VkLight.hpp>

#include <Rendering/Config.hpp>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	class VkDirectionalLight : public VkLight
	{
	public:
		//void Create(const IRenderInstance& _instance, const DLightInfos& _infos);

		//VkDescriptorBufferInfo CreateDescriptorBufferInfo() const noexcept override final;
	};

#endif
}

#endif // GUARD
