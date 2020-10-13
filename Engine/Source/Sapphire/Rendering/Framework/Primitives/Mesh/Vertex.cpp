// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/Vertex.hpp>

namespace Sa
{
	bool Vertex::operator == (const Vertex & _rhs) const noexcept
	{
		return position == _rhs.position && normal == _rhs.normal && texture == _rhs.texture;
	}

	bool Vertex::operator!=(const Vertex& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}

#if SA_RENDERING_API == SA_VULKAN

	const VkVertexInputBindingDescription* Vertex::GetBindingDescription() noexcept
	{
		static const VkVertexInputBindingDescription bindingDescriptions[bindingDescriptionNum]
		{
			VkVertexInputBindingDescription
			{
				0,										// binding.
				sizeof(Vertex),							// stride.
				VK_VERTEX_INPUT_RATE_VERTEX				// inputRate.
			}
		};

		return bindingDescriptions;
	}

	const VkVertexInputAttributeDescription* Vertex::GetAttributeDescriptions() noexcept
	{
		static const VkVertexInputAttributeDescription attributeDescriptions[attributeDescriptionNum]
		{
			VkVertexInputAttributeDescription
			{
				0,									// location.
				0,									// binding.
				VK_FORMAT_R32G32B32_SFLOAT,			// format.
				offsetof(Vertex, position)			// offset
			},

			VkVertexInputAttributeDescription
			{
				1,									// location.
				0,									// binding.
				VK_FORMAT_R32G32B32_SFLOAT,			// format.
				offsetof(Vertex, normal)			// offset
			},

			VkVertexInputAttributeDescription
			{
				2,									// location.
				0,									// binding.
				VK_FORMAT_R32G32B32_SFLOAT,			// format.
				offsetof(Vertex, tangent)			// offset
			},

			VkVertexInputAttributeDescription
			{
				3,									// location.
				0,									// binding.
				VK_FORMAT_R32G32B32_SFLOAT,			// format.
				offsetof(Vertex, bitangent)			// offset
			},

			VkVertexInputAttributeDescription
			{
				4,									// location.
				0,									// binding.
				VK_FORMAT_R32G32_SFLOAT,			// format.
				offsetof(Vertex, texture)			// offset
			},
		};

		return attributeDescriptions;
	}

#endif
}