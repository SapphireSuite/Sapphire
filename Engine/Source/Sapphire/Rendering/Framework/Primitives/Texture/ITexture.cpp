// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/ITexture.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>

namespace Sa
{
	ITexture* ITexture::CreateInstance(const IRenderInstance& _instance, const void* _data, uint32 _width, uint32 _height)
	{
#if SA_RENDERING_API == SA_VULKAN

		ITexture* result = new VkTexture;

#endif

		result->Create(_instance, _data, _width, _height);

		return result;
	}
}
