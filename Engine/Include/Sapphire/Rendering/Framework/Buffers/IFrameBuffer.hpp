// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IFRAME_BUFFER_GUARD
#define SAPPHIRE_RENDERING_IFRAME_BUFFER_GUARD

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	class IFrameBuffer : public IInterface
	{
	public:
		virtual void Begin() = 0;
		virtual void NextSubpass() = 0;
		virtual void End() = 0;
	};

	struct RenderFrame
	{
		const uint32 index = uint32(-1);
		IFrameBuffer& buffer;
	};
}

#endif // GUARD
