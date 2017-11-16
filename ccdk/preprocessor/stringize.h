#pragma once

//evaluate text
#define CCDK_PP_STRINGIZE(...) CCDK_PP_STRINGIZE_FAST(__VA_ARGS__)
#define CCDK_PP_STRINGIZE_FAST(...) #__VA_ARGS__

#define CCDK_PP_WSTRINGIZE(text) CCDK_PP_WSTRING_STEP1(text)
#define CCDK_PP_WSTRING_STEP1(text) CCDK_PP_WSTRING_STEP2(#text)
#define CCDK_PP_WSTRING_STEP2(text) L ## text
