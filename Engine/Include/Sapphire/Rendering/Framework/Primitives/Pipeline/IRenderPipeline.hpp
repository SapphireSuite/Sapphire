// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_IRENDER_PIPELINE_GUARD

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	class IRenderFrame;
	class IRenderInstance;
	class PipelineCreateInfos;

	class IRenderPipeline : public IInterface
	{
	public:
		virtual void Create(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual void Bind(const IRenderFrame& _frame) = 0;
	};
}

#endif // GUARD
