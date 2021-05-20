// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/RenderPass/SubPassDescriptor.hpp>

#include <Collections/Debug>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkAttachmentLoadOp API_GetAttachmentLoadOp(const AttachmentLoadMode _mode) noexcept
	{
		switch (_mode)
		{
			case AttachmentLoadMode::Clear:
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case AttachmentLoadMode::Load:
				return VK_ATTACHMENT_LOAD_OP_LOAD;
			case AttachmentLoadMode::None:
				return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			default:
				SA_LOG("Unknown AttachmentLoadMode!", Warning, Rendering);
				return VK_ATTACHMENT_LOAD_OP_CLEAR;
		}
	}

#endif
}
