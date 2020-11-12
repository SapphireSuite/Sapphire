// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IMESH_GUARD
#define SAPPHIRE_RENDERING_IMESH_GUARD

#include <Rendering/Framework/Primitives/IRenderPrimitive.hpp>
#include <Rendering/Framework/Primitives/Mesh/RawMesh.hpp>

namespace Sa
{
	class IRenderInstance;

	class IMesh : public IRenderPrimitive
	{
	protected:
		std::shared_ptr<VertexLayout> mLayout;

	public:
		std::shared_ptr<VertexLayout> GetLayout() const noexcept;
		
		virtual void Create(const IRenderInstance& _instance, const RawMesh& _rawMesh) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD
