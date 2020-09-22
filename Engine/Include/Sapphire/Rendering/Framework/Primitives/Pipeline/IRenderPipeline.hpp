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

		virtual void InitData(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset) = 0;
		virtual void UpdateData(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset) = 0;
	};
}

#endif // GUARD
