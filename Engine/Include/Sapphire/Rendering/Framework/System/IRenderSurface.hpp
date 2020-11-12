// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD

#include <Core/Types/IInterface.hpp>

#include <Rendering/Framework/Misc/RenderFormat.hpp>

namespace Sa
{
	class IRenderInstance;

	class IRenderPass;
	class RenderPassDescriptor;

	class FrameInfos;

	class IRenderSurface : public IInterface
	{
	public:
		virtual RenderFormat GetRenderFormat() const noexcept = 0;

		virtual void Create(const IRenderInstance& _instance) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual void Begin(const IRenderInstance& _instance) = 0;
		virtual void End(const IRenderInstance& _instance) = 0;

		virtual FrameInfos GetFrameInfos(uint32 _renderPassID) = 0;

		virtual uint32 AddRenderPass(IRenderInstance& _instance, const IRenderPass& _renderPass, const RenderPassDescriptor& _rpDescriptor) = 0;
		virtual void RemoveRenderPass(IRenderInstance& _instance, uint32 _renderPassID) = 0;
	};
}

#endif // GUARD
