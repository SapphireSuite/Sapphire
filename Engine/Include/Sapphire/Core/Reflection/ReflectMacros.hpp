// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_CORE_REFELCT_MACROS_GUARD
#define SAPPHIRE_CORE_REFELCT_MACROS_GUARD

#include <Core/Reflection/Primitives/Class.hpp>
#include <Core/Reflection/Reflector.hpp>

namespace Sa
{
	#define SA_ENUM()
	#define SA_PROP()
	#define SA_FUNC()
	#define SA_CLASS()
	#define SA_NAMESPACE()

	#define SA_REFLECT()


	// registered types.
	#define SA_PUSH_FIELD(_fType, _fName)\
		tInfos.Add(Field());
		//tInfos.Add(Field(#_fName, Reflector::GetType<_fType>(#_fType), offsetof(SA_CLASS_TYPE, _fName)));

	#define SA_FILL_TYPES()\
			SA_FILL_FIELDS()\
			SA_FILL_FUNCTIONS()

	#define SA_GENERATE_STRUCT_BODY()\
		static const Class& STypeInfos()\
		{\
			static Class tInfos;\
		\
			if(!tInfos.name.empty())\
				return tInfos;\
		\
			tInfos.name = SA_CLASS_NAME;\
			tInfos.fields.reserve(SA_FIELD_NUM);\
			tInfos.functions.reserve(SA_FUNCTION_NUM);\
		\
			Type* pushType = nullptr;\
		\
			SA_FILL_TYPES()\
		\
			return tInfos;\
		}\
		const Class& TypeInfos() const override\
		{\
			return STypeInfos();\
		}

	#define SA_GENERATE_CLASS_BODY()\
		public:\
			SA_GENERATE_STRUCT_BODY()\
		private:
}

#endif // GUARD
