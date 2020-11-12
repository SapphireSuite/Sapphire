// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexBindingLayout.hpp>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	uint32 VertexBindingLayout::GetBindingDescriptionNum() const
	{
		return 1u;
	}

	uint32 VertexBindingLayout::GetAttributeDescriptionNum() const
	{
		// Prefer desired description.
		const VertexLayout* layout = desiredLayout ? desiredLayout.get() : meshLayout.get();

		SA_ASSERT(layout, Nullptr, Rendering, L"Layout nullptr!");

		if (layout->comps == VertexComp::PNTanTexC)
			return 5u;
		else if (layout->comps == VertexComp::PNTanTex || layout->comps == VertexComp::PNTanC || layout->comps == VertexComp::PNTexC)
			return 4u;
		else if (layout->comps == VertexComp::PNTan || layout->comps == VertexComp::PNTex || layout->comps == VertexComp::PNC)
			return 3u;
		else if (layout->comps == VertexComp::PN || layout->comps == VertexComp::PTex || layout->comps == VertexComp::PC)
			return 2u;
		else if (layout->comps == VertexComp::Position)
			return 1u;

		return 0u;
	}

	std::unique_ptr<VkVertexInputBindingDescription> VertexBindingLayout::GetBindingDescription() const noexcept
	{
		// Only mesh layout can be bigger than desired.
		const VertexLayout* layout = meshLayout ? meshLayout.get() : desiredLayout.get();
		
		SA_ASSERT(layout, Nullptr, Rendering, L"Layout nullptr!");

		uint32 size = layout->vertexSize;

		return std::make_unique<VkVertexInputBindingDescription>(VkVertexInputBindingDescription
		{
			0,										// binding.
			size,									// stride.
			VK_VERTEX_INPUT_RATE_VERTEX				// inputRate.
		});
	}

	std::unique_ptr<VkVertexInputAttributeDescription[]> VertexBindingLayout::GetAttributeDescriptions() const noexcept
	{
		// Prefer desired description.
		const VertexLayout* layout = desiredLayout ? desiredLayout.get() : meshLayout.get();

		SA_ASSERT(layout, Nullptr, Rendering, L"layout nullptr!");


		/** 
		*	Layout to check offset.
		*	Adapt offset binding from mesh if provided.
		*/
		const VertexLayout* const offsetLayout = meshLayout ? meshLayout.get() : desiredLayout.get();

		uint32 index = 0u;
		std::unique_ptr<VkVertexInputAttributeDescription[]> result(new VkVertexInputAttributeDescription[GetAttributeDescriptionNum()]);

		if ((layout->comps & VertexComp::Position) != VertexComp::None)
		{
			SA_ASSERT((offsetLayout->comps & VertexComp::Position) != VertexComp::None, InvalidParam, Rendering, L"Try bind a missing component in mesh layout!");

			uint32 offset = offsetLayout->GetPositionOffet();

			result[index++] =
			{
				0,									// location.
				0,									// binding.
				VK_FORMAT_R32G32B32_SFLOAT,			// format.
				offset								// offset
			};
		}

		if ((layout->comps & VertexComp::Normal) != VertexComp::None)
		{
			SA_ASSERT((offsetLayout->comps & VertexComp::Normal) != VertexComp::None, InvalidParam, Rendering, L"Try bind a missing component in mesh layout!");

			uint32 offset = offsetLayout->GetNormalOffet();

			result[index++] =
			{
				1,									// location.
				0,									// binding.
				VK_FORMAT_R32G32B32_SFLOAT,			// format.
				offset								// offset
			};
		}

		if ((layout->comps & VertexComp::Tangent) != VertexComp::None)
		{
			SA_ASSERT((offsetLayout->comps & VertexComp::Tangent) != VertexComp::None, InvalidParam, Rendering, L"Try bind a missing component in mesh layout!");

			uint32 offset = offsetLayout->GetTangentOffet();

			result[index++] =
			{
				2,									// location.
				0,									// binding.
				VK_FORMAT_R32G32B32_SFLOAT,			// format.
				offset								// offset
			};
		}

		if ((layout->comps & VertexComp::Texture) != VertexComp::None)
		{
			SA_ASSERT((offsetLayout->comps & VertexComp::Texture) != VertexComp::None, InvalidParam, Rendering, L"Try bind a missing component in mesh layout!");

			uint32 offset = offsetLayout->GetTextureOffet();

			result[index++] =
			{
				3,									// location.
				0,									// binding.
				VK_FORMAT_R32G32_SFLOAT,			// format.
				offset								// offset
			};
		}

		if ((layout->comps & VertexComp::Color) != VertexComp::None)
		{
			SA_ASSERT((offsetLayout->comps & VertexComp::Color) != VertexComp::None, InvalidParam, Rendering, L"Try bind a missing component in mesh layout!");

			uint32 offset = offsetLayout->GetColorOffet();

			result[index++] =
			{
				4,									// location.
				0,									// binding.
				VK_FORMAT_R32G32B32_SFLOAT,			// format.
				offset								// offset
			};
		}


		return result;
	}

#endif
}
