// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IMESH_GUARD
#define SAPPHIRE_RENDERING_IMESH_GUARD

#include <Rendering/Framework/Primitives/IRenderPrimitive.hpp>

#include <Rendering/Framework/Primitives/Mesh/RawMesh.hpp>
#include <Rendering/Framework/Primitives/Mesh/MeshDrawInfos.hpp>

namespace Sa
{
	class IRenderInstance;
	class IFrameBuffer;

	class SA_ENGINE_API IMesh : public IRenderPrimitive
	{
	protected:
		std::shared_ptr<VertexLayout> mLayout;

	public:
		std::shared_ptr<VertexLayout> GetLayout() const noexcept;
		
		virtual void Create(const IRenderInstance& _instance, const RawMesh& _rawMesh);
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual void Draw(const IFrameBuffer& _frameBuffer, const MeshDrawInfos& _infos = MeshDrawInfos()) const = 0;
	};
}

#endif // GUARD
