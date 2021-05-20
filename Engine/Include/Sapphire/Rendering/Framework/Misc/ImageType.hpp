// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IMAGE_TYPE_GUARD
#define SAPPHIRE_RENDERING_IMAGE_TYPE_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/APIConfig.hpp>

namespace Sa
{
	enum class ImageType
	{
		/// Default 2D image.
		Image2D,

		Image3D,
		
		/// Cubemap.
		Cube,
	};

#if SA_RENDERING_API == SA_VULKAN

	VkImageType API_GetImageType(ImageType _type);

	VkImageViewType API_GetImageViewType(ImageType _type);

	uint32 API_GetLayerNum(ImageType _type);

#endif
}

#endif // GUARD
