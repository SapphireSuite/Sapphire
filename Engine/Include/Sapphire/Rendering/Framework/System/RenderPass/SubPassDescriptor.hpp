// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SUBPASS_DESCRIPTOR_GUARD
#define SAPPHIRE_RENDERING_SUBPASS_DESCRIPTOR_GUARD

#include <vector>

#include <Rendering/APIConfig.hpp>

#include <Rendering/Framework/Misc/Color.hpp>
#include <Rendering/Framework/Misc/Format.hpp>
#include <Rendering/Framework/System/RenderPass/SampleBits.hpp>

namespace Sa
{
	enum class AttachmentLoadMode
	{
		None,

		Load,

		Clear,
	};

	struct SubPassAttachmentDescriptor
	{
		Format format;

		// Used as input attachment in next subpass.
		bool bInputNext = false;

		AttachmentLoadMode loadMode = AttachmentLoadMode::Clear;
		Color clearColor = Color{ 0.0f, 0.0f, 0.05f, 0.0f };
	};

	struct SubPassDescriptor
	{
		// All color attachment must have the same sample count.
		SampleBits sampling = SampleBits::Sample1Bit;

		std::vector<SubPassAttachmentDescriptor> attachmentDescs;
	};

#if SA_RENDERING_API == SA_VULKAN

	VkAttachmentLoadOp API_GetAttachmentLoadOp(const AttachmentLoadMode _mode) noexcept;

#endif
}

#endif // GUARD
