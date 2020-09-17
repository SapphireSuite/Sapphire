// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_IINTERFACE_GUARD
#define SAPPHIRE_CORE_IINTERFACE_GUARD

#include <Collections/Debug>

namespace Sa
{
	class IInterface
	{
	public:
		virtual ~IInterface() = default;

		template <typename T>
		T& As();

		template <typename T>
		const T& As() const;
	};
}

#include <Core/Types/IInterface.inl>

#endif // GUARD
