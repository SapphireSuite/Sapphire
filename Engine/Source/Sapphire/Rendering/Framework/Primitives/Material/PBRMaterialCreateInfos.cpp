// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/PBRMaterialCreateInfos.hpp>

namespace Sa
{
	constexpr uint32 __albedoOffset = 2u;

	PBRMaterialCreateInfos::PBRMaterialCreateInfos(const IPipeline& _pipeline, const uint32 _descriptorSetNum) noexcept :
		MaterialCreateInfos(_pipeline, _descriptorSetNum)
	{
		bindings.reserve(2u * _descriptorSetNum + 1u);

		// UBO.
		{
			for (uint32 i = 0; i < _descriptorSetNum; ++i)
			{
				MaterialBindingInfos& camera = bindings.emplace_back();
				camera.binding = 0u;
				camera.descriptor = i;
			}

			for (uint32 i = 0; i < _descriptorSetNum; ++i)
			{
				MaterialBindingInfos& model = bindings.emplace_back();
				model.binding = 1u;
				model.descriptor = i;
			}
		}

		// Textures.
		MaterialBindingInfos& albedo = bindings.emplace_back();
		albedo.binding = 2u;
		albedo.SetImageSamplers2D({ nullptr,nullptr, nullptr, nullptr, }); // Albedo, roughness, metallic, ao.
	}

	void PBRMaterialCreateInfos::SetAlbedo(const ITexture& _albedo) noexcept
	{
		bindings[__albedoOffset * descriptorSetNum].SetImageSampler2D(_albedo, 0u);
	}

	void PBRMaterialCreateInfos::SetRoughness(const ITexture& _roughness) noexcept
	{
		bindings[__albedoOffset * descriptorSetNum].SetImageSampler2D(_roughness, 1u);
	}

	void PBRMaterialCreateInfos::SetMetallic(const ITexture& _metallic) noexcept
	{
		bindings[__albedoOffset * descriptorSetNum].SetImageSampler2D(_metallic, 2u);
	}

	void PBRMaterialCreateInfos::SetAmbientOccl(const ITexture& _ao) noexcept
	{
		bindings[__albedoOffset * descriptorSetNum].SetImageSampler2D(_ao, 3u);
	}
}
