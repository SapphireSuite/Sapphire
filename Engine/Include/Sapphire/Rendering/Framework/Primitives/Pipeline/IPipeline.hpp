// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IPIPELINE_GUARD
#define SAPPHIRE_RENDERING_IPIPELINE_GUARD

#include <Core/Types/IInterface.hpp>

#include <Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

#include <Rendering/Framework/Buffers/IFrameBuffer.hpp>

namespace Sa
{
	class IRenderInstance;

	class IPipeline : public IInterface
	{
	public:
		virtual void Create(const IRenderInstance& _instance, const PipelineCreateInfos& _infos) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual void Bind(const RenderFrame& _frame) = 0;
	};
}

#endif // GUARD
