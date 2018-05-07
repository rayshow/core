#pragma once

//OS
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define CCDK_OS_WINDOW
#	define CCDK_MAX_PATH        MAX_PATH
#	define CCDK_ANSI_LINE_TERMINATOR "\r\n"
#	define CCDK_WIDE_LINE_TERMINATOR L"\r\n"
#elif defined(linux) || defined(__linux) || defined(__linux__)
#   if defined(ANDROID) || defined(__ANDROID__)
#		define CCDK_OS_ANDROID
#   else
#		define CCDK_OS_LINUX
#   endif
#	define CCDK_MAX_PATH    PATH_MAX
#	define CCDK_ANSI_LINE_TERMINATOR "\n"
#	define CCDK_WIDE_LINE_TERMINATOR L"\n"
#elif  defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#	if defined(__IPHONEOS__)
#      define  CCDK_OS_APPLE
#   else
#      define  CCDK_OS_MAC
#   endif
#	define CCDK_MAX_PATH    1024
#	define CCDK_ANSI_LINE_TERMINATOR "\r"
#	define CCDK_WIDE_LINE_TERMINATOR L"\r"
#else
#error "OS not support"
#endif


//compiler
#if defined(_MSC_VER) && _MSC_VER >=1800
#	if defined(__clang__)
#		define CCDK_COMPILER_CLANG   //clang with msvc
#	else
#		define CCDK_COMPILER_MSVC
#	endif
#elif defined(__clang__) && __cplusplus >=201103L
#define CCDK_COMPILER_CLANG
#elif defined(__GNUC__) && !defined(__ibmxl__) && __cplusplus >=201103L
#define CCDK_COMPILER_GCC
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC) && __cplusplus >=201103L
#define CCDK_COMPILER_INTEL
#else
#error "compiler not support"
#endif

//window, linux, apple are all little endian
#define CCDK_LITTLE_ENDIAN                                       1
#define CCDK_CACHE_OPT                                           1

//archecture 
#if (defined(__x86_64__) || defined(_M_X64) || defined(_WIN64) \
  || defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) \
  || defined(__64BIT__) || defined(_LP64) || defined(__LP64__) \
  || defined(__ia64) || defined(__itanium__) || defined(_M_IA64) )  
#  define CCDK_ARCH64                                            1             
#else
#  define CCDK_ARCH64                                            0
#endif


//vector math support
#	if defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__x86_64__) || defined(__amd64__) //intel or amd
#		define CCDK_SSE                                         	 
#   elif  defined(_M_ARM) || defined(_M_ARM64) //mali 
#		define CCDK_NEON                          
#	else
#		define CCDK_FPU  //float compute
#	endif


//compile mode. debug or release
#if  defined(DEBUG) || defined _DEBUG
#define  CCDK_DEBUG												 1
#endif


//function like macro
#ifndef ccdk_likely
#if defined(CCDK_COMPILER_GCC) ||  defined(CCDK_COMPILER_CLANG) //linux or  clang
#define ccdk_likely(x)    __builtin_expect(!!(x), 1)
#define ccdk_unlikely(x)  __builtin_expect(!!(x), 0)
#else
#define ccdk_likely(x)   (x)
#define ccdk_unlikely(x) (x)
#endif
#endif

//function like macro
#if defined( CCDK_DEBUG )
#include<cassert>
#define ccdk_throw_at   false
#define ccdk_rethrow_at false
#define ccdk_assert( expr )  assert( expr )
#define ccdk_throw( expr ) ( assert(ccdk_throw_at), throw(expr))
#define ccdk_rethrow()     ( assert(ccdk_rethrow_at), throw )
#define ccdk_assert_if(expr) ccdk_assert(!(expr)); if(expr)
#else
#define ccdk_assert( expr ) 
#define ccdk_throw( expr )  throw(expr)
#define ccdk_rethrow()  throw
#endif


#if  defined(CCDK_COMPILER_MSVC)  
#	define CCDK_MS_ALIGN(n)     __declspec(align(n))
#	define CCDK_GCC_ALIGN(n) 
#	define CCDK_DLLEXPORT       __declspec(dllexport)    //building as a library
#	define CCDK_DLLIMPORT	    __declspec(dllimport)     //building with this library
#	define CCDK_DEPRECATED(version, msg) __declspec(deprecated(msg "please update your code"))
#	define CCDK_CDECL	        __cdecl			
#   define CCDK_FASTCALL        __fastcall
#   define CCDK_VECTORCALL      __vectorcall
#	define CCDK_STDCALL	        __stdcall										
#	define CCDK_FORCEINLINE     __forceinline						
#	define CCDK_FORCENOINLINE   __declspec(noinline)
#elif defined(CCDK_COMPILER_GCC) ||  defined(CCDK_COMPILER_CLANG) //linux or  clang
#	define CCDK_MS_ALIGN(n) 
#	define CCDK_GCC_ALIGN(n)	__attribute__((aligned(n)))
#	define CCDK_DLLEXPORT	__attribute__((visibility("default")))
#	define CCDK_DLLIMPORT	__attribute__((visibility("default")))
#	define CCDK_DEPRECATED(version, msg) __attribute__((deprecated(msg "please update your code")))
#	define CCDK_CDECL	  		
#	define CCDK_VARARGS    
#	define CCDK_STDCALL	  
#	define CCDK_FORCEINLINE inline __attribute__ ((always_inline))
#	define CCDK_FORCENOINLINE __attribute__((noinline))	
#else ///mac
#	define CCDK_MS_ALIGN(n) 
#	define CCDK_GCC_ALIGN(n)	__attribute__((aligned(n)))
#	define CCDK_DLLEXPORT	
#	define CCDK_DLLIMPORT	
#	define CCDK_DEPRECATED(version, msg) __attribute__((deprecated(MESSAGE "please update your code")))
#	define CCDK_CDECL	  		
#	define CCDK_VARARGS    
#	define CCDK_STDCALL	  
#	if  defined(CCDK_DEBUG)
#		define CCDK_FORCEINLINE inline
#	else
#		define CCDK_FORCEINLINE inline __attribute__ ((always_inline))
#	endif
#	define CCDK_FORCENOINLINE __attribute__((noinline))	
#endif

//lib or dll import export
#define CCDK_STATIC
#if defined(CCDK_STATIC)
#	define CCDK_API 
#else
#	if defined(CCDK_EXPORT_DLL)
#		define CCDK_API CCDK_DLLEXPORT
#   else 
#		define CCDK_API CCDK_DLLIMPORT
#	endif
#endif


#if defined(CCDK_COMPILER_MSVC)
//4514: un-used inline function had been removed
//4710: function had not been inlined
//4505: un-reference local function had been removed
#pragma warning(disable:4514 4710 4505)
#endif

#if defined(CCDK_COMPILER_MSVC) && (defined(DEBUG) || defined(_DEBUG))
//#define new  new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define ccdk_open_leak_check()                         \
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); \
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;                  \
	_CrtSetDbgFlag(tmpFlag);                          
	//_CrtDumpMemoryLeaks();
#else
#define ccdk_open_leak_check()
#endif









