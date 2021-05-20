// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/RenderPass/SampleBits.hpp>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkSampleCountFlagBits API_GetSampleCount(SampleBits _sampleBits)
	{
		return static_cast<VkSampleCountFlagBits>(_sampleBits);
	}

#endif
}
