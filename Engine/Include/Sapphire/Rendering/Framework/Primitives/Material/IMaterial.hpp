// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IMATERIAL_GUARD
#define SAPPHIRE_RENDERING_IMATERIAL_GUARD

#include <Rendering/Framework/Primitives/IRenderPrimitive.hpp>

#include <Rendering/Framework/Primitives/Material/MaterialCreateInfos.hpp>

namespace Sa
{
	class IRenderInstance;

	class IMaterial : public IRenderPrimitive
	{
	public:
		virtual void Create(const IRenderInstance& _instance, const MaterialCreateInfos& _infos) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual void Bind(const RenderFrame& _frame, const IPipeline& _pipeline) = 0;
	};
}

#endif // GUARD

