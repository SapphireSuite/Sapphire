// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_CREATE_INFOS_GUARD

#include <Rendering/Framework/Primitives/Pipeline/IPipeline.hpp>
#include <Rendering/Framework/Primitives/Shader/ShaderBindingType.hpp>

namespace Sa
{
	struct MaterialBindingInfos
	{
		uint32 binding = 0u;

		ShaderBindingType type = ShaderBindingType::UniformBuffer;

		// Size of buffer data.
		uint64 bufferDataSize = 0u;

		// Either IBuffer or ITexture.
		std::vector<IInterface*> buffers;
	};

	struct SA_ENGINE_API MaterialCreateInfos
	{
		const IPipeline& pipeline;

		std::vector<MaterialBindingInfos> bindings;

		MaterialCreateInfos(const IPipeline& _pipeline) noexcept;
	};
}

#endif // GUARD
