// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VERTEX_BINDING_LAYOUT_GUARD
#define SAPPHIRE_RENDERING_VERTEX_BINDING_LAYOUT_GUARD

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexLayout.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

namespace Sa
{
	struct SA_ENGINE_API VertexBindingLayout
	{
		std::shared_ptr<const VertexLayout> desiredLayout = nullptr;
		std::shared_ptr<const VertexLayout> meshLayout = nullptr;

#if SA_RENDERING_API == SA_VULKAN

		uint32 GetBindingDescriptionNum() const;

		uint32 GetAttributeDescriptionNum() const;

		std::unique_ptr<VkVertexInputBindingDescription> GetBindingDescription() const noexcept;

		std::unique_ptr<VkVertexInputAttributeDescription[]> GetAttributeDescriptions() const noexcept;

#endif
	};
}

#endif // GUARD
