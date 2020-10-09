// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_MATERIAL_GUARD
#define SAPPHIRE_RENDERING_IRENDER_MATERIAL_GUARD

#include <Rendering/Framework/Primitives/IRenderPrimitive.hpp>

namespace Sa
{
	class IRenderFrame;
	class IRenderInstance;
	class IRenderPipeline;

	struct RenderMaterialCreateInfos;

	class IRenderMaterial : public IRenderPrimitive
	{
		virtual IRenderPipeline& GetPipeline(uint32 _index) = 0;
		virtual const IRenderPipeline& GetPipeline(uint32 _index) const = 0;

	public:
		virtual void Create(const IRenderInstance& _instance, const RenderMaterialCreateInfos& _matCreateInfos) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Bind(const IRenderFrame& _frame, uint32 _renderPassIndex) const;
		
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API virtual void InitVariable(const IRenderInstance& _instance, uint32 _renderPassIndex,
			const void* _data, uint32 _dataSize, uint32 _offset = 0);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API virtual void UpdateVariable(const IRenderInstance& _instance, const IRenderFrame& _frame,
			uint32 _renderPassIndex, const void* _data, uint32 _dataSize, uint32 _offset = 0);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API static IRenderMaterial* CreateInstance();
	};
}

#endif // GUARD

