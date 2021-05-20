// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Mesh/RawMesh.hpp>

namespace Sa
{
	RawMesh::RawMesh(VertexComp _comps) noexcept :
		mLayout{ VertexLayout::Make(_comps) }
	{
	}

	std::shared_ptr<VertexLayout> RawMesh::GetLayout() const noexcept
	{
		return mLayout;
	}

	void RawMesh::SetLayout(VertexComp _comps)
	{
		// No conversion to perform.
		if (mLayout->comps == VertexComp::None || mLayout->comps == _comps)
		{
			mLayout = VertexLayout::Make(_comps);
			return;
		}

		// TODO: Implement layout conversion.
		SA_ASSERT(false, NotImplemented, Rendering, L"Method not implemented yet!")

		mLayout = VertexLayout::Make(_comps);
	}

	void RawMesh::ComputeTangents()
	{
		SA_ASSERT((mLayout->comps & VertexComp::Texture) != VertexComp::None,
			InvalidParam, Rendering, L"Mesh need texture channel to compute tangents!");

		for (uint32 i = 0; i + 2 < SizeOf(indices); i += 3)
		{
			const Vec3f& APosition = *mLayout->Position(vertices.data(), indices[i]);
			const Vec3f& ATexture = *mLayout->Texture(vertices.data(), indices[i]);

			const Vec3f& BPosition = *mLayout->Position(vertices.data(), indices[i + 1]);
			const Vec3f& BTexture = *mLayout->Texture(vertices.data(), indices[i + 1]);

			const Vec3f& CPosition = *mLayout->Position(vertices.data(), indices[i + 2]);
			const Vec3f& CTexture = *mLayout->Texture(vertices.data(), indices[i + 2]);

			Vec3f edge1 = BPosition - APosition;
			Vec3f edge2 = CPosition - APosition;

			Vec2f deltaUV1 = BTexture - ATexture;
			Vec2f deltaUV2 = CTexture - ATexture;

			float ratio = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;

			if (Maths::Equals0(ratio))
				continue;

			float f = 1.0f / ratio;

			Vec3f tangent = (f * Vec3f(deltaUV2.y * edge1.x - deltaUV1.y * edge2.x,
				deltaUV2.y * edge1.y - deltaUV1.y * edge2.y,
				deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)).Normalize();

			// Average tangent.
			*mLayout->Tangent(vertices.data(), indices[i]) += tangent;
			*mLayout->Tangent(vertices.data(), indices[i + 1]) += tangent;
			*mLayout->Tangent(vertices.data(), indices[i + 2]) += tangent;
		}


		for (uint32 i = 0; i < SizeOf(vertices) / mLayout->vertexSize; ++i)
			mLayout->Tangent(vertices.data(), i)->Normalize();
	}
}
