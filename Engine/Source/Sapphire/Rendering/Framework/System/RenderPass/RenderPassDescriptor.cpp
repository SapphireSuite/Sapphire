// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/RenderPass/RenderPassDescriptor.hpp>

#include <Rendering/Framework/System/IRenderSurface.hpp>

namespace Sa
{
	RenderPassDescriptor RenderPassDescriptor::CreateDefaultForward(const IRenderSurface* _surface)
	{
		RenderPassDescriptor result{};

		// === Main Subpass ===
		result.subPassDescs.resize(1u);

		result.subPassDescs[0].attachmentDescs.resize(1u);

		if (_surface)
		{
			result.bPresent = true;
			result.subPassDescs[0].attachmentDescs[0].format = _surface->GetFormat();
		}
		else
		{
			result.bPresent = false;
			result.subPassDescs[0].attachmentDescs[0].format = Format::sRGBA_32;
		}

		return result;
	}

	RenderPassDescriptor RenderPassDescriptor::CreateDefaultPBRDeferred(const IRenderSurface* _surface)
	{
		RenderPassDescriptor result{};

		result.sampling = SampleBits::Sample1Bit;

		result.subPassDescs.resize(2u);

		// === PBR Subpass ===
		result.subPassDescs[0].attachmentDescs.resize(4u);

		// Position.
		result.subPassDescs[0].attachmentDescs[0].format = Format::RGBA_32;
		
		// Normal
		result.subPassDescs[0].attachmentDescs[1].format = Format::RGBA_32;

		// Albedo
		result.subPassDescs[0].attachmentDescs[2].format = Format::RGBA_32;

		// PBR: Metallic, Roughness, Ambiant occlusion.
		result.subPassDescs[0].attachmentDescs[3].format = Format::RGBA_32;


		// === Present Subpass ===
		result.subPassDescs[1].attachmentDescs.resize(1u);

		if (_surface)
		{
			result.bPresent = true;
			result.subPassDescs[1].attachmentDescs[0].format = _surface->GetFormat();
		}
		else
		{
			result.bPresent = false;
			result.subPassDescs[1].attachmentDescs[0].format = Format::sRGBA_32;
		}


		return result;
	}
}