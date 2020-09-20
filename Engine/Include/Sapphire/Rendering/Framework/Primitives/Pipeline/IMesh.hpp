// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IMESH_GUARD
#define SAPPHIRE_RENDERING_IMESH_GUARD

#include <Core/Types/IInterface.hpp>

#include <Rendering/Framework/Primitives/Pipeline/Vertex.hpp>

namespace Sa
{
	class IRenderInstance;
	class IRenderFrame;

	class IMesh : public IInterface
	{
	protected:
		virtual void Create(const IRenderInstance& _instance,
			const std::vector<Vertex>& _vertices,
			const std::vector<uint32>& _indices) = 0;
	public:
		static IMesh* CreateInstance(const IRenderInstance& _instance,
			const std::vector<Vertex>& _vertices,
			const std::vector<uint32>& _indices);
		
		virtual void Destroy(const IRenderInstance& _instance) = 0;

		virtual void Draw(const IRenderFrame& _frame) const = 0;
	};
}

#endif // GUARD
