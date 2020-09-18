// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_ISHADER_GUARD
#define SAPPHIRE_RENDERING_ISHADER_GUARD

#include <string>

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	class IRenderInstance;

	class IShader : public IInterface
	{
	public:
		virtual void Create(const IRenderInstance& _instance, const std::wstring& _fileName) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD
