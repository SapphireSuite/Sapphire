// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_WINDOW_IWINDOW_GUARD
#define SAPPHIRE_WINDOW_IWINDOW_GUARD

#include <Core/Types/Int.hpp>
#include <Core/Types/IInterface.hpp>
#include <Core/Types/Variadics/Event.hpp>

#include <Window/Config.hpp>

namespace Sa
{
	class IRenderInstance;

	class SA_ENGINE_API IWindow : public IInterface
	{
	public:
		mutable Event<void(const IWindow&, uint32, uint32)> onResizeEvent;

		virtual void Create(uint32 _width, uint32 _height, const char* _name = "Main Window") = 0;
		virtual void Destroy() = 0;

		virtual void Update() = 0;

		virtual bool ShouldClose() const = 0;


		static void Init();
		static void UnInit();

		static void GetRequiredExtensions(std::vector<const char*>& _extensions) noexcept;
	};
}

#endif // GUARD
