// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/ICubemap.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Vulkan/Primitives/Texture/VkCubemap.hpp>

namespace Sa
{
	ICubemap* ICubemap::main = nullptr;


	ICubemap* ICubemap::CreateInstance(const IRenderInstance& _instance, const RawCubemap& _rawCubemap)
	{
		ICubemap* result = nullptr;

#if SA_RENDERING_API == SA_VULKAN

		result = new VkCubemap;

#endif

		SA_ASSERT(result, Nullptr, Rendering, L"Cubemap creation failed! API implemenation may be missing.");

		result->Create(_instance, _rawCubemap);

		return result;
	}
}
