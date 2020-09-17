// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_RENDERING_IMESH_GUARD
#define SAPPHIRE_RENDERING_IMESH_GUARD

#include <Core/Types/IInterface.hpp>

#include <Rendering/Framework/Model/Vertex.hpp>

namespace Sa
{
	class IRenderInstance;

	class IMesh : public IInterface
	{
	public:
		virtual void Create(const IRenderInstance& _instance, const std::vector<Vertex>& _vertices) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD
