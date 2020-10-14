// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/MaterialShaders.hpp>

#include <Core/Algorithms/SizeOf.hpp>

namespace Sa
{
	// Force init before MaterialShaders::default.
	SpecConstantInfos SpecConstantInfos::defaultVertex;
	SpecConstantInfos SpecConstantInfos::defaultFragment
	{
		// Alpha model.
		{ 0u, sizeof(int) },

		// Illumination model.
		{ 1u, sizeof(int) },
	};

	const MaterialShaders MaterialShaders::default
	{
		{ nullptr, SpecConstantInfos::defaultVertex, ShaderType::Vertex },
		{ nullptr, SpecConstantInfos::defaultFragment, ShaderType::Fragment },
	};
}
