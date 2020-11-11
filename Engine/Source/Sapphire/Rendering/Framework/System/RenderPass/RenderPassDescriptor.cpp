// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/RenderPass/RenderPassDescriptor.hpp>

namespace Sa
{
	RenderPassDescriptor CreateDefaultForward()
	{
		RenderPassDescriptor result{};

		result.bPresent = true;

		
		// === Main Subpass ===
		result.subPassDescriptors.resize(1u);

		result.subPassDescriptors[0].attachmentDescriptors.resize(1u);
		result.subPassDescriptors[0].attachmentDescriptors[0].format = RenderFormat{ 4u, RenderFormatType::sRGB };

		return result;
	}

	RenderPassDescriptor CreateDefaultPBRDeferred()
	{
		RenderPassDescriptor result{};

		result.bPresent = true;

		result.subPassDescriptors.resize(2u);

		// === PBR Subpass ===
		result.subPassDescriptors[0].attachmentDescriptors.resize(4u);

		// Position.
		result.subPassDescriptors[0].attachmentDescriptors[0].format = RenderFormat{ 4u, RenderFormatType::RGB };
		
		// Normal
		result.subPassDescriptors[0].attachmentDescriptors[1].format = RenderFormat{ 4u, RenderFormatType::RGB };

		// Albedo
		result.subPassDescriptors[0].attachmentDescriptors[2].format = RenderFormat{ 4u, RenderFormatType::RGB };

		// PBR: Metallic, Roughness, Ambiant occlusion.
		result.subPassDescriptors[0].attachmentDescriptors[3].format = RenderFormat{ 4u, RenderFormatType::RGB };


		// === Present Subpass ===
		result.subPassDescriptors[1].attachmentDescriptors.resize(1u);
		result.subPassDescriptors[1].attachmentDescriptors[0].format = RenderFormat{ 4u, RenderFormatType::sRGB };


		return result;
	}

	const RenderPassDescriptor RenderPassDescriptor::defaultForward = CreateDefaultForward();
	const RenderPassDescriptor RenderPassDescriptor::defaultPBRDeferred = CreateDefaultPBRDeferred();
}