// Copyright 2020 Sapphire development team. All Rights Reserved.

#ifndef SAPPHIRE_CORE_OBJECT_GUARD
#define SAPPHIRE_CORE_OBJECT_GUARD

#include <Core/Reflection/Primitives/Class.hpp>

namespace Sa
{
	class Object
	{
	public:
		SA_ENGINE_API static const Class& STypeInfos();
		SA_ENGINE_API virtual const Class& TypeInfos() const;

		SA_ENGINE_API std::string Serialize();
		SA_ENGINE_API void Deserialize(const std::string& str);
	};
}

#endif // GUARD
