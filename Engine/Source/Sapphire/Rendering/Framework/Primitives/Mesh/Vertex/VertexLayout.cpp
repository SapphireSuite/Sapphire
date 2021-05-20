// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexLayout.hpp>
#include <Rendering/Framework/Primitives/Mesh/Vertex/VertexLayoutSpec.hpp>

namespace Sa
{
	VertexLayout::VertexLayout(VertexComp _comps, uint32 _vertexSize) noexcept :
		comps{ _comps },
		vertexSize{ _vertexSize }
	{
	}


	char* VertexLayout::Prev(char* _data) const
	{
		SA_ASSERT(vertexSize != 0u, InvalidParam, Rendering, L"Null vertex layout!");

		return _data - vertexSize;
	}

	char* VertexLayout::Next(char* _data) const
	{
		SA_ASSERT(vertexSize != 0u, InvalidParam, Rendering, L"Null vertex layout!");

		return _data + vertexSize;
	}


	std::shared_ptr<VertexLayout> VertexLayout::Make(VertexComp _comps) noexcept
	{
		switch (_comps)
		{
			case Sa::VertexComp::Position:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::Position>);
			case Sa::VertexComp::PN:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PN>);
			case Sa::VertexComp::PTex:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PTex>);
			case Sa::VertexComp::PC:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PC>);
			case Sa::VertexComp::PNTan:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PNTan>);
			case Sa::VertexComp::PNTex:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PNTex>);
			case Sa::VertexComp::PNC:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PNC>);
			case Sa::VertexComp::PNTanTex:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PNTanTex>);
			case Sa::VertexComp::PNTanC:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PNTanC>);
			case Sa::VertexComp::PNTexC:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PNTexC>);
			case Sa::VertexComp::PNTanTexC:
				return std::shared_ptr<VertexLayout>(new VertexLayoutSpec<VertexComp::PNTanTexC>);
			default:
				SA_ASSERT(false, InvalidParam, Rendering, L"Invalid vertex layout!");
				break;
		}

		return nullptr;
	}
}
