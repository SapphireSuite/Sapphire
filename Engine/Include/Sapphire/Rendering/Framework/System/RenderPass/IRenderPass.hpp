// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_PASS_GUARD
#define SAPPHIRE_RENDERING_IRENDER_PASS_GUARD

#include <Core/Types/IInterface.hpp>

#include <Rendering/Framework/System/RenderPass/RenderPassCreateInfos.hpp>

namespace Sa
{
	class IRenderInstance;
	class IRenderSurface;
	class IRenderFrame;

	class IRenderPass : public IInterface
	{
	public:
		// TODO: CLEAN LATER
		SA_ENGINE_API static IRenderPass* mainRenderPass;

		virtual void SetClearColor(const Color& _color) = 0;

		virtual void Create(const IRenderInstance& _instance, const IRenderSurface& _surface, const RenderPassCreateInfos& _createInfos) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual void Begin(const IRenderFrame& _frame) = 0;
		virtual void End(const IRenderFrame& _frame) = 0;
	};
}

#endif // GUARD
