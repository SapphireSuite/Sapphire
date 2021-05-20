// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD

#include <Core/Types/IInterface.hpp>

#include <Rendering/Framework/Misc/Format.hpp>

namespace Sa
{
	class IRenderInstance;

	class IRenderPass;
	class RenderPassDescriptor;

	struct RenderFrame;
	class IFrameBuffer;

	class IRenderSurface : public IInterface
	{
	public:
		virtual Format GetFormat() const noexcept = 0;

		virtual void Create(const IRenderInstance& _instance) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual RenderFrame Begin(const IRenderInstance& _instance) = 0;
		virtual void End(const IRenderInstance& _instance) = 0;

		virtual const std::vector<IFrameBuffer*>& CreateFrameBuffers(IRenderInstance& _instance, const IRenderPass& _renderPass,
			const RenderPassDescriptor& _renderPassDesc, uint32* _size = nullptr) = 0;
		virtual void DestroyFrameBuffers(IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD
