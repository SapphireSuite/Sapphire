// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Buffers/FrameBufferCreateInfos.hpp>

namespace Sa
{
	FrameBufferCreateInfos::FrameBufferCreateInfos(const IRenderPass& _renderPass, const RenderPassDescriptor& _rpDescriptor) noexcept :
		renderPass{ _renderPass },
		rpDescriptor{ _rpDescriptor }
	{
	}
}
