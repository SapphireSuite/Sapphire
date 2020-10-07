// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_MATERIAL_GUARD
#define SAPPHIRE_RENDERING_IRENDER_MATERIAL_GUARD

#include <Rendering/Framework/Primitives/IRenderPrimitive.hpp>

namespace Sa
{
	class IRenderFrame;
	class IRenderPipeline;
	class IRenderInstance;

	class PipelineCreateInfos;

	class IRenderMaterial : public IRenderPrimitive
	{
	protected:
		IRenderPipeline* mPipeline = nullptr;

	public:
		virtual void Create(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos) = 0;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API virtual void Destroy(const IRenderInstance& _instance);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API virtual void Bind(const IRenderFrame& _frame) const;
		
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API virtual void InitVariable(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset = 0);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API virtual void UpdateVariable(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset = 0);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API static IRenderMaterial* CreateInstance();
	};
}

#endif // GUARD

