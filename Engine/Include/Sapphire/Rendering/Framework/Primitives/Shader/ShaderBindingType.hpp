// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SHADER_BINDING_TYPE_GUARD
#define SAPPHIRE_RENDERING_SHADER_BINDING_TYPE_GUARD

#include <Rendering/APIConfig.hpp>

namespace Sa
{
	enum class ShaderBindingType
	{
		UniformBuffer,

		StorageBuffer,

		ImageSampler2D,

		ImageSamplerCube,

		InputAttachment
	};

#if SA_RENDERING_API == SA_VULKAN

	VkDescriptorType API_GetDescriptorType(ShaderBindingType _type);

#endif
}

#endif // GUARD
