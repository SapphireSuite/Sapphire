// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/RenderPass/RenderPassCreateInfos.hpp>

namespace Sa
{
	RenderPassCreateInfos CreateDefaultForward()
	{
		RenderPassCreateInfos result;

		result.bPresent = true;

		
		// === Main Subpass ===
		result.subPassInfos.resize(1u);

		result.subPassInfos[0].attachmentInfos.resize(1u);
		result.subPassInfos[0].attachmentInfos[0].format = RenderFormat{ 4u, RenderFormatType::sRGB };

		return result;
	}

	RenderPassCreateInfos CreateDefaultPBRDeferred()
	{
		RenderPassCreateInfos result;

		result.bPresent = true;

		result.subPassInfos.resize(2u);

		// === PBR Subpass ===
		result.subPassInfos[0].attachmentInfos.resize(4u);

		// Position.
		result.subPassInfos[0].attachmentInfos[0].format = RenderFormat{ 4u, RenderFormatType::RGB };
		
		// Normal
		result.subPassInfos[0].attachmentInfos[1].format = RenderFormat{ 4u, RenderFormatType::RGB };

		// Albedo
		result.subPassInfos[0].attachmentInfos[2].format = RenderFormat{ 4u, RenderFormatType::RGB };

		// PBR: Metallic, Roughness, Ambiant occlusion.
		result.subPassInfos[0].attachmentInfos[3].format = RenderFormat{ 4u, RenderFormatType::RGB };


		// === Present Subpass ===
		result.subPassInfos[1].attachmentInfos.resize(1u);
		result.subPassInfos[1].attachmentInfos[0].format = RenderFormat{ 4u, RenderFormatType::sRGB };


		return result;
	}

	const RenderPassCreateInfos RenderPassCreateInfos::defaultForward = CreateDefaultForward();
	const RenderPassCreateInfos RenderPassCreateInfos::defaultPBRDeferred = CreateDefaultPBRDeferred();
}