#pragma once

//evaluate text
#define A3D_PP_STRINGIZE(...) A3D_PP_STRINGIZE_FAST(__VA_ARGS__)
#define A3D_PP_STRINGIZE_FAST(...) #__VA_ARGS__

#define A3D_PP_WSTRINGIZE(text) A3D_PP_WSTRING_STEP1(text)
#define A3D_PP_WSTRING_STEP1(text) A3D_PP_WSTRING_STEP2(#text)
#define A3D_PP_WSTRING_STEP2(text) L ## text