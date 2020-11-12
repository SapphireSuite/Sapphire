// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VERTEX_COMP_GUARD
#define SAPPHIRE_RENDERING_VERTEX_COMP_GUARD

namespace Sa
{
	enum class VertexComp : uint8
	{
		None = 0,

		Position = 1 << 0,

		Normal = 1 << 1,

		Tangent = 1 << 2,

		Texture = 1 << 3,
		
		Color = 1 << 4,


		// === Groups ===
		PN = Position | Normal,
		PTex = Position | Texture,
		PC = Position | Color,

		PNTan = Position | Normal | Tangent,
		PNTex = Position | Normal | Texture,
		PNC = Position | Normal | Color,

		PNTanTex = Position | Normal | Tangent | Texture,
		PNTanC = Position | Normal | Tangent | Color,
		PNTexC = Position | Normal | Texture | Color,

		PNTanTexC = Position | Normal | Tangent | Texture | Color,

		Default = PNTanTex,
	};
}

#endif // GUARD
