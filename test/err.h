test_mpl_function.cpp: In function ‘int main()’:
test_mpl_function.cpp:321:22: error: no match for call to ‘(ccdk::mpl::fn::member_function_t<int (ccdk::test_copy_t::*)(int, const char*), ccdk::test_copy_t, int, ccdk::test_copy_t, int, const char*>) (int, const char [11])’
  mfn1(1, "bind mfn1 ");
                      ^
In file included from test_mpl_function.cpp:17:0:
../ccdk/mpl/function/bind_mfn.h:30:34: note: candidate: constexpr Ret ccdk::mpl::fn::member_function_t<Fn, Class, Ret, Args>::operator()(Args ...) const [with Fn = int (ccdk::test_copy_t::*)(int, const char*); Class = ccdk::test_copy_t; Ret = int; Args = {ccdk::test_copy_t, int, const char*}]
   CCDK_FORCEINLINE constexpr Ret operator()(Args... args) const noexcept
                                  ^~~~~~~~
../ccdk/mpl/function/bind_mfn.h:30:34: note:   candidate expects 3 arguments, 2 provided
test_mpl_function.cpp:322:22: error: no match for call to ‘(ccdk::mpl::fn::member_function_t<int (ccdk::test_copy_t::*)(int, const char*), ccdk::test_copy_t, int, ccdk::test_copy_t, int, const char*>) (int, const char [11])’
  mfn2(2, "bind mfn2 ");
                      ^
In file included from test_mpl_function.cpp:17:0:
../ccdk/mpl/function/bind_mfn.h:30:34: note: candidate: constexpr Ret ccdk::mpl::fn::member_function_t<Fn, Class, Ret, Args>::operator()(Args ...) const [with Fn = int (ccdk::test_copy_t::*)(int, const char*); Class = ccdk::test_copy_t; Ret = int; Args = {ccdk::test_copy_t, int, const char*}]
   CCDK_FORCEINLINE constexpr Ret operator()(Args... args) const noexcept
                                  ^~~~~~~~
../ccdk/mpl/function/bind_mfn.h:30:34: note:   candidate expects 3 arguments, 2 provided
test_mpl_function.cpp:323:22: error: no match for call to ‘(ccdk::mpl::fn::member_function_t<int (ccdk::test_copy_t::*)(int, const char*), ccdk::test_copy_t, int, ccdk::test_copy_t, int, const char*>) (int, const char [11])’
  mfn3(3, "bind mfn3 ");
                      ^
In file included from test_mpl_function.cpp:17:0:
../ccdk/mpl/function/bind_mfn.h:30:34: note: candidate: constexpr Ret ccdk::mpl::fn::member_function_t<Fn, Class, Ret, Args>::operator()(Args ...) const [with Fn = int (ccdk::test_copy_t::*)(int, const char*); Class = ccdk::test_copy_t; Ret = int; Args = {ccdk::test_copy_t, int, const char*}]
   CCDK_FORCEINLINE constexpr Ret operator()(Args... args) const noexcept
                                  ^~~~~~~~
../ccdk/mpl/function/bind_mfn.h:30:34: note:   candidate expects 3 arguments, 2 provided
test_mpl_function.cpp:324:22: error: no match for call to ‘(ccdk::mpl::fn::member_function_t<int (ccdk::test_copy_t::*)(int, const char*), ccdk::test_copy_t, int, ccdk::test_copy_t, int, const char*>) (int, const char [11])’
  mfn4(4, "bind mfn4 ");
                      ^
In file included from test_mpl_function.cpp:17:0:
../ccdk/mpl/function/bind_mfn.h:30:34: note: candidate: constexpr Ret ccdk::mpl::fn::member_function_t<Fn, Class, Ret, Args>::operator()(Args ...) const [with Fn = int (ccdk::test_copy_t::*)(int, const char*); Class = ccdk::test_copy_t; Ret = int; Args = {ccdk::test_copy_t, int, const char*}]
   CCDK_FORCEINLINE constexpr Ret operator()(Args... args) const noexcept
                                  ^~~~~~~~
../ccdk/mpl/function/bind_mfn.h:30:34: note:   candidate expects 3 arguments, 2 provided
