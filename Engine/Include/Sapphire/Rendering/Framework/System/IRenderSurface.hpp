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

	class IRenderSurface : public IInterface
	{
	public:
		virtual RenderFormat GetRenderFormat() const noexcept = 0;

		virtual void Create(const IRenderInstance& _instance) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual void AddRenderPass(const IRenderInstance& _instance, const IRenderPass& _renderPass, const RenderPassDescriptor& _rpDescriptor) = 0;
		virtual void RemoveRenderPass(const IRenderInstance& _instance, const IRenderPass& _renderPass) = 0;
	};
}

#endif // GUARD
