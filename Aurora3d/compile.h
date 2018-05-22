#pragma once

#include<ccdk/compile.h>

#define A3D_OS_DEFINE  CCDK_OS_DEFINE
#define A3D_OS_WINDOW  CCDK_OS_WINDOW
#define A3D_OS_ANDROID CCDK_OS_ANDROID
#define A3D_OS_LINUX   CCDK_OS_LINUX
#define A3D_OS_APPLE   CCDK_OS_APPLE
#define A3D_OS_MAC     CCDK_OS_MAC

#define A3D_COMPILER_DEFINE CCDK_COMPILER_DEFINE
#define A3D_COMPILER_MSVC   CCDK_COMPILER_MSVC
#define A3D_COMPILER_CLANG  CCDK_COMPILER_CLANG
#define A3D_COMPILER_GCC    CCDK_COMPILER_GCC
#define A3D_COMPILER_INTEL  CCDK_COMPILER_INTEL

#define A3D_MAX_PATH CCDK_MAX_PATH
#define A3D_ANSI_LINE_TERMINATOR CCDK_ANSI_LINE_TERMINATOR
#define A3D_WIDE_LINE_TERMINATOR CCDK_WIDE_LINE_TERMINATOR

#define A3D_LITTLE_ENDIAN CCDK_LITTLE_ENDIAN

#define A3D_ARCH64 CCDK_ARCH64

#define A3D_DEBUG CCDK_DEBUG

#define a3d_likely   ccdk_likely
#define a3d_unlikely ccdk_unlikely

#define a3d_assert( expr )    ccdk_assert( expr )
#define a3d_throw( expr )     ccdk_throw( expr )
#define a3d_rethrow()         ccdk_rethrow()
#define a3d_assert_if( expr ) ccdk_assert_if(expr)

#define A3D_DLLEXPORT CCDK_DLLEXPORT
#define A3D_DLLIMPORT CCDK_DLLIMPORT
#define A3D_STATIC
#if defined(A3D_STATIC)
#	define A3D_API 
#else
#	if defined(CCDK_EXPORT_DLL)
#		define CCDK_API A3D_DLLEXPORT
#   else 
#		define CCDK_API A3D_DLLIMPORT
#	endif
#endif
#define A3D_DEPRECATED(version, msg) CCDK_DEPRECATED(version, msg)

#define A3D_CDECL          CCDK_CDECL
#define A3D_FASTCALL       CCDK_FASTCALL
#define A3D_VECTORCALL     CCDK_VECTORCALL
#define A3D_STDCALL        CCDK_STDCALL
#define A3D_FORCEINLINE    CCDK_FORCEINLINE
#define A3D_FORCENOINLINE  CCDK_FORCENOINLINE

#define a3d_open_leak_check ccdk_open_leak_check

