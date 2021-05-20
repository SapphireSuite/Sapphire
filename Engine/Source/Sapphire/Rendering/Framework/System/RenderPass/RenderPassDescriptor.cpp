// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/RenderPass/RenderPassDescriptor.hpp>

#include <Rendering/Framework/System/IRenderSurface.hpp>

namespace Sa
{
	RenderPassDescriptor RenderPassDescriptor::DefaultSingle(const IRenderSurface* _surface)
	{
		RenderPassDescriptor result{};

		result.subPassDescs.reserve(1u);

		// === Main Subpass ===
		{
			SubPassDescriptor& mainSubpassDesc = result.subPassDescs.emplace_back();
			mainSubpassDesc.sampling = SampleBits::Sample8Bits;

			mainSubpassDesc.attachmentDescs.reserve(2u);


			// Color and present attachment.
			SubPassAttachmentDescriptor& colorAttachDesc = mainSubpassDesc.attachmentDescs.emplace_back();
			colorAttachDesc.format = _surface ? _surface->GetFormat() : Format::sRGBA_32;


			// Depth attachment.
			SubPassAttachmentDescriptor& depthAttachDesc = mainSubpassDesc.attachmentDescs.emplace_back();
			depthAttachDesc.format = Format::Depth_16;
		}

		return result;
	}

	RenderPassDescriptor RenderPassDescriptor::DefaultPBR(const IRenderSurface* _surface)
	{
		RenderPassDescriptor result{};

		result.subPassDescs.reserve(2u);

		// === PBR Subpass ===
		{
			SubPassDescriptor& pbrSubpassDesc = result.subPassDescs.emplace_back();
			pbrSubpassDesc.sampling = SampleBits::Sample8Bits;

			// Deferred position attachment.
			SubPassAttachmentDescriptor& posAttachDesc = pbrSubpassDesc.attachmentDescs.emplace_back();
			posAttachDesc.format = Format::RGBA_32;
			posAttachDesc.bInputNext = true;

			// Deferred normal attachment.
			SubPassAttachmentDescriptor& normAttachDesc = pbrSubpassDesc.attachmentDescs.emplace_back();
			normAttachDesc.format = Format::RGBA_32;
			normAttachDesc.bInputNext = true;

			// Deferred albedo attachment.
			SubPassAttachmentDescriptor& albedoAttachDesc = pbrSubpassDesc.attachmentDescs.emplace_back();
			albedoAttachDesc.format = Format::RGBA_32;
			albedoAttachDesc.bInputNext = true;

			// Deferred PBR (Metallic, Roughness, Ambiant occlusion) attachment.
			SubPassAttachmentDescriptor& pbrAttachDesc = pbrSubpassDesc.attachmentDescs.emplace_back();
			pbrAttachDesc.format = Format::RGBA_32;
			pbrAttachDesc.bInputNext = true;

			SubPassAttachmentDescriptor& depthAttachDesc = pbrSubpassDesc.attachmentDescs.emplace_back();
			depthAttachDesc.format = Format::Depth_16;
		}


		// === Present Subpass ===
		{
			SubPassDescriptor& presentSubpassDesc = result.subPassDescs.emplace_back();
			presentSubpassDesc.sampling = SampleBits::Sample8Bits;

			SubPassAttachmentDescriptor& presentAttachDesc = presentSubpassDesc.attachmentDescs.emplace_back();
			presentAttachDesc.format = _surface ? _surface->GetFormat() : Format::sRGBA_32;
		}


		return result;
	}
}