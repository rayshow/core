#pragma once

//make event macro useful, or event macro called with empty
#define AURORA3D_EVENT

//check c++ version
#if __cplusplus < 201103L && _MSC_VER<1800
#	error "only compile with standard greater or equal to c++11!"
#endif

//check platform
#if !defined(linux) && !defined(__linux) && !defined(__linux__) && \
	!defined(_WIN32) && !defined(_WIN64) && !defined(_MSC_VER) && \
		!defined(__APPLE__) || defined(__arm__)
#	error "platform not support!"
#endif

//archecture 
#if (defined(__x86_64__) || defined(_M_X64) || defined(_WIN64) \
  || defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) \
  || defined(__64BIT__) || defined(_LP64) || defined(__LP64__) \
  || defined(__ia64) || defined(__itanium__) || defined(_M_IA64) )  
#  define AURORA3D_ARCH64                                            1             
#else
#  define AURORA3D_ARCH64                                            0
#endif

//window, linux, apple are all little endian
#define AURORA3D_LITTLE_ENDIAN                                       1


//platform. window, linux, android, apple, mac
#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER) 
#	define  AURORA3D_DESKTOP										 1
#	define  AURORA3D_DX11
#	define  AURORA3D_WINDOW
#	undef max
#	undef min
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	define AURORA3D_LINUX
#   if defined(ANDROID) || defined(__ANDROID__)
#		define AURORA3D_DESKTOP                                      0
#   else
#		define AURORA3D_DESKTOP                                      1
#   endif
#elif defined(__APPLE__)
#	define AURORA3D_MAC
#	if defined(__IPHONEOS__)
#		define AURORA3D_DESKTOP                                      0
#   else 
#		define AURORA3D_DESKTOP                                      1
#   endif
#else
#   error "platform not support!"
#endif

//compile mode. debug and release
#if  defined(DEBUG) || defined _DEBUG
#define  AURORA3D_DEBUG												1
#endif

#define  WSTR(str)  L##str

#if defined(AURORA3D_WINDOW) && defined(_MSC_VER)  ///ms-vs
#	define A3D_MS_ALIGN(n)     __declspec(align(n))
#	define A3D_GCC_ALIGN(n) 
#	define A3D_DLLEXPORT       __declspec(dllexport)    //building as a library
#	define A3D_DLLIMPORT	   __declspec(dllimport)    //building with this library
#	define A3D_DEPRECATED(version, msg) __declspec(deprecated(msg "please update your code"))
#	define A3D_LIKELY(state)   (state)
#	define A3D_UNLIKELY(state) (state)
#	define A3D_CDECL	       __cdecl			
#	define A3D_VARARGS         __cdecl			
#	define A3D_STDCALL	       __stdcall										
#	define A3D_FORCEINLINE     __forceinline						
#	define A3D_FORCENOINLINE   __declspec(noinline)
#	define A3D_MAX_PATH        MAX_PATH
#	define AURORA3D_SSE        
#	define A3D_ANSI_LINE_TERMINATOR "\r\n"
#	define A3D_WIDE_LINE_TERMINATOR WSTR("\r\n")
#elif defined(AURORA3D_LINUX) && defined(__clang__) ///linux or window clang
#	define A3D_MS_ALIGN(n) 
#	define A3D_GCC_ALIGN(n)	__attribute__((aligned(n)))
#	define A3D_DLLEXPORT	__attribute__((visibility("default")))
#	define A3D_DLLIMPORT	__attribute__((visibility("default")))
#	define A3D_DEPRECATED(version, msg) __attribute__((deprecated(MESSAGE "please update your code")))
#	define A3D_LIKELY(state)   __builtin_expect(!!(x),1)
#	define A3D_UNLIKELY(state) __builtin_expect(!!(x),0)
#	define A3D_CDECL	  		
#	define A3D_VARARGS    
#	define A3D_STDCALL	  
#	define A3D_FORCEINLINE inline __attribute__ ((always_inline))
#	define A3D_FORCENOINLINE __attribute__((noinline))	
#	define A3D_MAX_PATH    PATH_MAX
#	if defined(_M_IX86) || defined(__i386__) || defined(_M_X64)
	|| defined(__x86_64__) || defined (__amd64__) 
#		define AURORA3D_SSE                                         	 
#   elif  defined(_M_ARM) || defined(_M_ARM64) //mali 
#		define AURORA3D_NEON                          
#	else
#		define AURORA3D_FPU  //float compute
#	endif
#	define A3D_ANSI_LINE_TERMINATOR "\n"
#	define A3D_WIDE_LINE_TERMINATOR WSTR("\n")
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
#	define A3D_MAX_PATH    1024
#   define AURORA3D_SSE 
#	define A3D_ANSI_LINE_TERMINATOR "\r"
#	define A3D_WIDE_LINE_TERMINATOR WSTR("\r")
#endif

//compile constant 
#if __cplusplus >= 201103L || _MSC_VER >=1900
#define CONSTEXPR constexpr
#else
#define CONSTEXPR const
#endif

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





