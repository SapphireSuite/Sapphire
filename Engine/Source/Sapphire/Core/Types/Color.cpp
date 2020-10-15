// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Types/Color.hpp>

namespace Sa
{
	const Color Color::red = Color{ 1.0f, 0.0f, 0.0f, 1.0f };
	const Color Color::green = Color{ 0.0f, 1.0f, 0.0f, 1.0f };
	const Color Color::blue = Color{ 1.0f, 0.0f, 1.0f, 1.0f };
	const Color Color::black = Color{ 0.0f, 0.0f, 0.0f, 1.0f };
	const Color Color::white = Color{ 1.0f, 1.0f, 1.0f, 1.0f };

	const Color Color::defaultClearColor = Color{ 0.0f, 0.0f, 0.05f, 1.0f };
}
