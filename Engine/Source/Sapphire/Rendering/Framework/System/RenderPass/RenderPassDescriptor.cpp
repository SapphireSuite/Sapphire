// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/RenderPass/RenderPassDescriptor.hpp>

#include <Rendering/Framework/System/IRenderSurface.hpp>

namespace Sa
{
	RenderPassDescriptor RenderPassDescriptor::CreateDefaultForward(const IRenderSurface* _surface)
	{
		RenderPassDescriptor result{};

		// === Main Subpass ===
		result.subPassDescriptors.resize(1u);

		result.subPassDescriptors[0].attachmentDescriptors.resize(1u);

		if (_surface)
		{
			result.bPresent = true;
			result.subPassDescriptors[0].attachmentDescriptors[0].format = _surface->GetFormat();
		}
		else
		{
			result.bPresent = false;
			result.subPassDescriptors[0].attachmentDescriptors[0].format = Format::sRGBA_32;
		}

		return result;
	}

	RenderPassDescriptor RenderPassDescriptor::CreateDefaultPBRDeferred(const IRenderSurface* _surface)
	{
		RenderPassDescriptor result{};

		result.subPassDescriptors.resize(2u);

		// === PBR Subpass ===
		result.subPassDescriptors[0].attachmentDescriptors.resize(4u);

		// Position.
		result.subPassDescriptors[0].attachmentDescriptors[0].format = Format::RGBA_32;
		
		// Normal
		result.subPassDescriptors[0].attachmentDescriptors[1].format = Format::RGBA_32;

		// Albedo
		result.subPassDescriptors[0].attachmentDescriptors[2].format = Format::RGBA_32;

		// PBR: Metallic, Roughness, Ambiant occlusion.
		result.subPassDescriptors[0].attachmentDescriptors[3].format = Format::RGBA_32;


		// === Present Subpass ===
		result.subPassDescriptors[1].attachmentDescriptors.resize(1u);

		if (_surface)
		{
			result.bPresent = true;
			result.subPassDescriptors[1].attachmentDescriptors[0].format = _surface->GetFormat();
		}
		else
		{
			result.bPresent = false;
			result.subPassDescriptors[1].attachmentDescriptors[0].format = Format::sRGBA_32;
		}


		return result;
	}
}