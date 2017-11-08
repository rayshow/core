
#define  A3D_MPL_BINARY_OP_DECL(OpName, InnerOp)                                                                              \
		template<typename T,typenameprefix,... TArgs> struct OpName :public T::template InnerOp<typename OpName<TArgsprefix,...>::type >{}; \
		template<typename T1, typename T2> struct OpName<T1, T2> :public T1::template InnerOp<T2>{};                          \
		template<typename T> struct OpName<T> { typedef T type; };
