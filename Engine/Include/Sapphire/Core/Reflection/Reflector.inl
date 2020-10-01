// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
	template <typename T>
	const Type& Reflector::GetType(const char* _tName)
	{
		Type* type = FindType(_tName);

		// Type not found.
		if (!type)
		{
			type = new TypeTraits<T>(_tName);

			AddType(_tName, type);
		}

		return *type;
	}
}
