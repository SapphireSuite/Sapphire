// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_CREATE_INFOS_GUARD

#include <Rendering/Framework/Primitives/Pipeline/IPipeline.hpp>
#include <Rendering/Framework/Primitives/Shader/ShaderBindingType.hpp>

namespace Sa
{
	class IBuffer;
	class ITexture;

	struct SA_ENGINE_API MaterialBindingInfos
	{
		uint32 binding = 0u;

		ShaderBindingType type = ShaderBindingType::UniformBuffer;

		// Size of buffer data (used for buffers).
		uint64 bufferDataSize = 0u;

		union
		{
			// UBO.
			std::vector<IBuffer*> buffers;

			// Image sampler.
			std::vector<ITexture*> textures;

			// Storage buffer.
			IBuffer* storageBuffer;
		};

		MaterialBindingInfos() noexcept;
		MaterialBindingInfos(MaterialBindingInfos&& _other) noexcept;
		MaterialBindingInfos(const MaterialBindingInfos& _other) noexcept;
		~MaterialBindingInfos() noexcept;

		MaterialBindingInfos& operator=(MaterialBindingInfos&& _rhs) noexcept;
		MaterialBindingInfos& operator=(const MaterialBindingInfos& _rhs) noexcept;
	};

	struct SA_ENGINE_API MaterialCreateInfos
	{
		const IPipeline& pipeline;

		std::vector<MaterialBindingInfos> bindings;

		MaterialCreateInfos(const IPipeline& _pipeline) noexcept;
	};
}

#endif // GUARD
