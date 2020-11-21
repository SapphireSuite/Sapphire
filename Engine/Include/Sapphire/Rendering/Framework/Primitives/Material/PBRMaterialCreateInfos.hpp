// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_PBR_MATERIAL_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_PBR_MATERIAL_CREATE_INFOS_GUARD

#include <Rendering/Framework/Primitives/Material/MaterialCreateInfos.hpp>

namespace Sa
{
	struct PBRMaterialCreateInfos : public MaterialCreateInfos
	{
		PBRMaterialCreateInfos(const IPipeline& _pipeline, const uint32 _descriptorSetNum = 1u) noexcept;

		void SetAlbedo(const ITexture& _albedo) noexcept;
		void SetRoughness(const ITexture& _roughness) noexcept;
		void SetMetallic(const ITexture& _metallic) noexcept;
		void SetAmbientOccl(const ITexture& _ao) noexcept;
	};
}

#endif // GUARD
