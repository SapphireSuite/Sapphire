// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Core/Reflection/Reflector.hpp>

#include <Collections/Debug>

namespace Sa
{
	Reflector Reflector::sInstance;

	Reflector::~Reflector()
	{
		for (auto it = types.begin(); it != types.end(); ++it)
		{
			SA_ASSERT(it->second, Nullptr, Reflection, L"Destroy nullptr type!");

			delete it->second;
		}
	}

	Type* Reflector::FindType(const char* _tName)
	{
		auto find = sInstance.types.find(_tName);

		if (find != sInstance.types.end())
		{
			SA_ASSERT(find->second, Nullptr, Reflection, L"Find nullptr type!");

			return find->second;
		}

		return nullptr;
	}

	void Reflector::AddType(const char* _tName, Type* _type)
	{
		sInstance.types.emplace(_tName, _type);
	}
}