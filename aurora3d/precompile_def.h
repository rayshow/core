#pragma once

//make event macro useful, or event macro called with empty
#define AURORCCDK_EVENT

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
#  define AURORCCDK_ARCH64                                            1             
#else
#  define AURORCCDK_ARCH64                                            0
#endif

//window, linux, apple are all little endian
#define AURORCCDK_LITTLE_ENDIAN                                       1


//platform. window, linux, android, apple, mac
#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER) 
#	define  AURORCCDK_DESKTOP										 1
#	define  AURORCCDK_DX11
#	define  AURORCCDK_WINDOW
#	undef max
#	undef min
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	define AURORCCDK_LINUX
#   if defined(ANDROID) || defined(__ANDROID__)
#		define AURORCCDK_DESKTOP                                      0
#   else
#		define AURORCCDK_DESKTOP                                      1
#   endif
#elif defined(__APPLE__)
#	define AURORCCDK_MAC
#	if defined(__IPHONEOS__)
#		define AURORCCDK_DESKTOP                                      0
#   else 
#		define AURORCCDK_DESKTOP                                      1
#   endif
#else
#   error "platform not support!"
#endif

//compile mode. debug and release
#if  defined(DEBUG) || defined _DEBUG
#define  AURORCCDK_DEBUG												1
#endif

#define  WSTR(str)  L##str

#if defined(AURORCCDK_WINDOW) && defined(_MSC_VER)  ///ms-vs
#	define CCDK_MS_ALIGN(n)     __declspec(align(n))
#	define CCDK_GCC_ALIGN(n) 
#	define CCDK_DLLEXPORT       __declspec(dllexport)    //building as a library
#	define CCDK_DLLIMPORT	   __declspec(dllimport)    //building with this library
#	define CCDK_DEPRECATED(version, msg) __declspec(deprecated(msg "please update your code"))
#	define CCDK_LIKELY(state)   (state)
#	define CCDK_UNLIKELY(state) (state)
#	define CCDK_CDECL	       __cdecl			
#	define CCDK_VARARGS         __cdecl			
#	define CCDK_STDCALL	       __stdcall										
#	define CCDK_FORCEINLINE     __forceinline						
#	define CCDK_FORCENOINLINE   __declspec(noinline)
#	define CCDK_MAX_PATH        MAX_PATH
#	define AURORCCDK_SSE        
#	define CCDK_ANSI_LINE_TERMINATOR "\r\n"
#	define CCDK_WIDE_LINE_TERMINATOR WSTR("\r\n")
#elif defined(AURORCCDK_LINUX) && defined(__clang__) ///linux or window clang
#	define CCDK_MS_ALIGN(n) 
#	define CCDK_GCC_ALIGN(n)	__attribute__((aligned(n)))
#	define CCDK_DLLEXPORT	__attribute__((visibility("default")))
#	define CCDK_DLLIMPORT	__attribute__((visibility("default")))
#	define CCDK_DEPRECATED(version, msg) __attribute__((deprecated(MESSAGE "please update your code")))
#	define CCDK_LIKELY(state)   __builtin_expect(!!(x),1)
#	define CCDK_UNLIKELY(state) __builtin_expect(!!(x),0)
#	define CCDK_CDECL	  		
#	define CCDK_VARARGS    
#	define CCDK_STDCALL	  
#	define CCDK_FORCEINLINE inline __attribute__ ((always_inline))
#	define CCDK_FORCENOINLINE __attribute__((noinline))	
#	define CCDK_MAX_PATH    PATH_MAX
#	if defined(_M_IX86) || defined(__i386__) || defined(_M_X64)
	|| defined(__x86_64__) || defined (__amd64__) 
#		define AURORCCDK_SSE                                         	 
#   elif  defined(_M_ARM) || defined(_M_ARM64) //mali 
#		define AURORCCDK_NEON                          
#	else
#		define AURORCCDK_FPU  //float compute
#	endif
#	define CCDK_ANSI_LINE_TERMINATOR "\n"
#	define CCDK_WIDE_LINE_TERMINATOR WSTR("\n")
#else ///mac
#	define CCDK_MS_ALIGN(n) 
#	define CCDK_GCC_ALIGN(n)	__attribute__((aligned(n)))
#	define CCDK_DLLEXPORT	
#	define CCDK_DLLIMPORT	
#	define CCDK_DEPRECATED(version, msg) __attribute__((deprecated(MESSAGE "please update your code")))
#	define CCDK_LIKELY(state)   
#	define CCDK_UNLIKELY(state) 
#	define CCDK_CDECL	  		
#	define CCDK_VARARGS    
#	define CCDK_STDCALL	  
#	if  defined(AURORCCDK_DEBUG)
#		define CCDK_FORCEINLINE inline
#	else
#		define CCDK_FORCEINLINE inline __attribute__ ((always_inline))
#	endif
#	define CCDK_FORCENOINLINE __attribute__((noinline))	
#	define CCDK_MAX_PATH    1024
#   define AURORCCDK_SSE 
#	define CCDK_ANSI_LINE_TERMINATOR "\r"
#	define CCDK_WIDE_LINE_TERMINATOR WSTR("\r")
#endif

//compile constant 
#if __cplusplus >= 201103L || _MSC_VER >=1900
#define CONSTEXPR constexpr
#else
#define CONSTEXPR const
#endif

#define AURORCCDK_STATIC
#if defined(AURORCCDK_STATIC)
#	define AURORCCDK_API 
#else
#	if defined(AURORCCDK_EXPORT_DLL)
#		define AURORCCDK_API CCDK_DLLEXPORT
#   else 
#		define AURORCCDK_API CCDK_DLLIMPORT
#	endif
#endif





