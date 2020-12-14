// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDERPASS_DESCRIPTOR_GUARD
#define SAPPHIRE_RENDERING_RENDERPASS_DESCRIPTOR_GUARD

#include <Rendering/Framework/System/RenderPass/SubPassDescriptor.hpp>

namespace Sa
{
	class IRenderSurface;

	class SA_ENGINE_API RenderPassDescriptor
	{
	public:
		std::vector<SubPassDescriptor> subPassDescs;

		/**
		*	\brief Create a default single-pass forward rendering renderpass descriptor.
		*/
		static RenderPassDescriptor DefaultSingle(const IRenderSurface* _surface = nullptr);

		/**
		*	\brief Create a default multi-pass PBR deferred rendering renderpass descriptor.
		*/
		static RenderPassDescriptor DefaultPBR(const IRenderSurface* _surface = nullptr);
	};
}

#endif // GUARD