#pragma once 

#include<core/mpl/type_traits/impl/has_operator_type.h>
#include<core/mpl/type_traits/impl/has_member_fn_decl.h>

namespace core
{
	namespace mpl
	{
		//if define T::operator[](int) will not has  right value  if call T::operator[](float) which is allowed
		//TODO : check  parameter and Ret can implicitly convert to defined function parameters

		//e.g. 
		//defined TestOperator::operator[](int)
		//defined typedef int (TestOperator::*CallType)(float);
		//CallType t = &TestOperator::operator[];  will be error
		//but TestOperator::operator[](float) is allow
		HAS_MEMBER_FN_DECL(operator[], indexer);
	}
}