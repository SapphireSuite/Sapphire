// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_CREATE_INFOS_GUARD

#include <Rendering/Framework/Primitives/Pipeline/IPipeline.hpp>

#include <Rendering/Framework/Primitives/Material/MaterialBindingInfos.hpp>

namespace Sa
{
	struct SA_ENGINE_API MaterialCreateInfos
	{
		const IPipeline& pipeline;
		const uint32 descriptorSetNum = 0u;

		std::vector<MaterialBindingInfos> bindings;

		MaterialCreateInfos(const IPipeline& _pipeline, const uint32 _descriptorSetNum = 1u) noexcept;
	};
}

#endif // GUARD
