#pragma once

#include<core/mpl/type_traits/declval.h>

namespace core
{
	namespace mpl
	{
			//test type
			struct no_op { char pad[1]; };
			struct has_op { char pad[2]; };
			inline no_op operator ,(no_op, has_op) { return declval<no_op>(); };

			// (NoOperation, HasVoidReturn) => NoOperation
			// (void, HasVoidReturn)  =>HasVoidReturn         
			// used to check return type
			struct has_void { char pad[2]; };
			template<typename T> inline no_op operator,(T, const has_void&) { return declval<no_op>(); };

			// (NoOperation,HasAnyReturn) => NoOperation
			// (T, HasAnyReturn)          => T
			// (void, HasAnyReturn)       => HasAnyReturn
			// used to check return is void
			struct has_any { char pad[2]; };
			template<typename T> inline T operator,(const T&, const has_any&) { return declval<T>(); };
			inline no_op operator,(const no_op&, const has_any&) { return declval<no_op>(); };

#define HasOperationValue(T)  (sizeof(T) == sizeof(has_op))

	}
}