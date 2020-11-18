// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_FRAME_BUFFER_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_FRAME_BUFFER_CREATE_INFOS_GUARD

#include <Maths/Space/Vector2.hpp>

namespace Sa
{
	class IRenderPass;
	struct RenderPassDescriptor;

	struct FrameBufferCreateInfos
	{
		const IRenderPass& renderPass;
		const RenderPassDescriptor& rpDescriptor;

		Vec2ui extent;
		uint32 poolIndex = 0u;
		uint32 layerNum = 1u;

		FrameBufferCreateInfos(const IRenderPass& renderPass, const RenderPassDescriptor& rpDescriptor) noexcept;
	};
}

#endif // GUARD
