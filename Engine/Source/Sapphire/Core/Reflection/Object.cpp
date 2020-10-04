// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Reflection/Object.hpp>

namespace Sa
{
	const Class& Object::STypeInfos()
	{
		static Class tInfos;

		if (!tInfos.name.empty())
			return tInfos;

		tInfos.name = "Object";
		tInfos.fields.reserve(0);
		tInfos.functions.reserve(0);

		return tInfos;
	}

	const Class& Object::TypeInfos() const
	{
		return STypeInfos();
	}

	std::string Object::Serialize()
	{
		return TypeInfos().Serialize(static_cast<void*>(this));
	}

	void Object::Deserialize(const std::string& str)
	{
		const char* temp = str.data();
		TypeInfos().Deserialize(static_cast<void*>(this), temp);
	}
}
