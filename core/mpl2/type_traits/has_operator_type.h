#pragma once

#include<core/mpl/type_traits/declval.h>

namespace Aurora3D
{
	namespace mpl
	{
			//test type
			struct NoOperation { char pad[1]; };
			struct HasOperation { char pad[2]; };
			inline NoOperation operator ,(NoOperation, HasOperation) { return Declval<NoOperation>(); };

			// (NoOperation, HasVoidReturn) => NoOperation
			// (void, HasVoidReturn)  =>HasVoidReturn         
			// used to check return type
			struct HasVoidReturn { char pad[2]; };
			template<typename T> inline NoOperation operator,(T, const HasVoidReturn&) { return Declval<NoOperation>(); };

			// (NoOperation,HasAnyReturn) => NoOperation
			// (T, HasAnyReturn)          => T
			// (void, HasAnyReturn)       => HasAnyReturn
			// used to check return is void
			struct HasAnyReturn { char pad[2]; };
			template<typename T> inline T operator,(const T&, const HasAnyReturn&) { return Declval<T>(); };
			inline NoOperation operator,(const NoOperation&, const HasAnyReturn&) { return Declval<NoOperation>(); };


			template<typename T, T t>
			struct OperatorPlaceHolder {};


#define HasOperationValue(T)  (sizeof(T) == sizeof(HasOperation))

	}
}