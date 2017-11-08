#pragma once

//OS
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define AURORA3D_OS_WINDOW
#	define A3D_MAX_PATH        MAX_PATH
#	define A3D_ANSI_LINE_TERMINATOR "\r\n"
#	define A3D_WIDE_LINE_TERMINATOR WSTR("\r\n")
#elif defined(linux) || defined(__linux) || defined(__linux__)
#   if defined(ANDROID) || defined(__ANDROID__)
#		define AURORA3D_OS_ANDROID
#   else
#		define AURORA3D_OS_LINUX
#   endif
#	define A3D_MAX_PATH    PATH_MAX
#	define A3D_ANSI_LINE_TERMINATOR "\n"
#	define A3D_WIDE_LINE_TERMINATOR WSTR("\n")
#elif  defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#	if defined(__IPHONEOS__)
#      define  AURORA3D_OS_APPLE
#   else
#      define  AURORA3D_OS_MAC
#   endif
#	define A3D_MAX_PATH    1024
#	define A3D_ANSI_LINE_TERMINATOR "\r"
#	define A3D_WIDE_LINE_TERMINATOR L##"\r"
#else
#error "OS not support"
#endif


//compiler
#if defined(_MSC_VER) && _MSC_VER>=1800
#define AURORA3D_COMPILER_MSVC
#elif defined(__clang__) && __cplusplus >=201103L
#define AURORA3D_COMPILER_CLANG
#elif defined(__GNUC__) && !defined(__ibmxl__) && __cplusplus >=201103L
#define AURORA3D_COMPILER_GCC
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC) && __cplusplus >=201103L
#define AURORA3D_COMPILER_INTEL
#else
#error "compiler not support"
#endif

//window, linux, apple are all little endian
#define AURORA3D_LITTLE_ENDIAN                                       1
#define AURORA3D_CACHE_OPT                                           1

//archecture 
#if (defined(__x86_64__) || defined(_M_X64) || defined(_WIN64) \
  || defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) \
  || defined(__64BIT__) || defined(_LP64) || defined(__LP64__) \
  || defined(__ia64) || defined(__itanium__) || defined(_M_IA64) )  
#  define AURORA3D_ARCH64                                            1             
#else
#  define AURORA3D_ARCH64                                            0
#endif


//vector math support
#	if defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__x86_64__) || defined(__amd64__) //intel or amd
#		define AURORA3D_SSE                                         	 
#   elif  defined(_M_ARM) || defined(_M_ARM64) //mali 
#		define AURORA3D_NEON                          
#	else
#		define AURORA3D_FPU  //float compute
#	endif


//compile mode. debug or release
#if  defined(DEBUG) || defined _DEBUG
#define  AURORA3D_DEBUG												 1
#endif


#if  defined(AURORA3D_COMPILER_MSVC)  
#	define A3D_MS_ALIGN(n)     __declspec(align(n))
#	define A3D_GCC_ALIGN(n) 
#	define A3D_DLLEXPORT       __declspec(dllexport)    //building as a library
#	define A3D_DLLIMPORT	   __declspec(dllimport)    //building with this library
#	define A3D_DEPRECATED(version, msg) __declspec(deprecated(msg "please update your code"))
#	define A3D_LIKELY(state)   (state)
#	define A3D_UNLIKELY(state) (state)
#	define A3D_CDECL	       __cdecl			
#   define A3D_FASTCALL        __fastcall
#   define A3D_VECTORCALL      __vectorcall
#	define A3D_STDCALL	       __stdcall										
#	define A3D_FORCEINLINE     __forceinline						
#	define A3D_FORCENOINLINE   __declspec(noinline)
#elif defined(A3D_COMPILER_GCC) ||  defined(A3D_COMPILER_CLANG) //linux or  clang
#	define A3D_MS_ALIGN(n) 
#	define A3D_GCC_ALIGN(n)	__attribute__((aligned(n)))
#	define A3D_DLLEXPORT	__attribute__((visibility("default")))
#	define A3D_DLLIMPORT	__attribute__((visibility("default")))
#	define A3D_DEPRECATED(version, msg) __attribute__((deprecated(msg "please update your code")))
#	define A3D_LIKELY(state)   __builtin_expect(!!(x),1)
#	define A3D_UNLIKELY(state) __builtin_expect(!!(x),0)
#	define A3D_CDECL	  		
#	define A3D_VARARGS    
#	define A3D_STDCALL	  
#	define A3D_FORCEINLINE inline __attribute__ ((always_inline))
#	define A3D_FORCENOINLINE __attribute__((noinline))	
#else ///mac
#	define A3D_MS_ALIGN(n) 
#	define A3D_GCC_ALIGN(n)	__attribute__((aligned(n)))
#	define A3D_DLLEXPORT	
#	define A3D_DLLIMPORT	
#	define A3D_DEPRECATED(version, msg) __attribute__((deprecated(MESSAGE "please update your code")))
#	define A3D_LIKELY(state)   
#	define A3D_UNLIKELY(state) 
#	define A3D_CDECL	  		
#	define A3D_VARARGS    
#	define A3D_STDCALL	  
#	if  defined(AURORA3D_DEBUG)
#		define A3D_FORCEINLINE inline
#	else
#		define A3D_FORCEINLINE inline __attribute__ ((always_inline))
#	endif
#	define A3D_FORCENOINLINE __attribute__((noinline))	
#endif

//lib or dll import export
#define AURORA3D_STATIC
#if defined(AURORA3D_STATIC)
#	define AURORA3D_API 
#else
#	if defined(AURORA3D_EXPORT_DLL)
#		define AURORA3D_API A3D_DLLEXPORT
#   else 
#		define AURORA3D_API A3D_DLLIMPORT
#	endif
#endif


#if defined(AURORA3D_COMPILER_MSVC)
//4514: un-used inline function had been removed
//4710: function had not been inlined
//4505: un-reference local function had been removed
#pragma warning(disable:4514 4710 4505)
#endif









