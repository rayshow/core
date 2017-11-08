#pragma once

#include<functional>
#include"../common_def.h"

namespace Aurora3D
{
	//flexible than HardCode find mode
	typedef std::function<bool(achar)>  AChar1stAppear;
	typedef std::function<bool(wchar)>  WChar1stAppear;

	typedef std::function<bool(achar,uint32)>  ACharNstAppear;
	typedef std::function<bool(wchar,uint32)>  WCharNstAppear;

	//support char type
	template<typename T> struct IsSupportedCharset{ enum{ value = false}; };
	template<> struct IsSupportedCharset<achar>{ enum{ value = true }; };
	template<> struct IsSupportedCharset<wchar>{ enum{ value = true }; };

	//for some pattern search
	template<typename T> struct Char1stAppearCondi{};
	template<> struct Char1stAppearCondi<AString>   { typedef AChar1stAppear        type; };
	template<> struct Char1stAppearCondi<WString>   { typedef WChar1stAppear        type; };
	
	template<typename T> struct CharNstAppearCondi{};
	template<> struct CharNstAppearCondi<AString>   { typedef ACharNstAppear        type; };
	template<> struct CharNstAppearCondi<WString>   { typedef WCharNstAppear        type; };

	//get char container
	template<typename T> struct CharContainerType{};
	template<> struct CharContainerType<achar>   { typedef AString        type; };
	template<> struct CharContainerType<wchar>   { typedef WString        type; };

	//base type
	template<typename T> struct StringBaseType{};
	template<> struct StringBaseType<AString>   { typedef achar       type; };
	template<> struct StringBaseType<WString>   { typedef wchar       type; };

	// StringParse<T> support
	template<uint32 T >
	struct Catalogy{ enum{ value = T }; };

	//None Type don't support parse, BaseType use StringOperation:FromString(T),
	//ComposeType use T.FromString() and compose with BaseType
	//support StringParse::parse(T)
	namespace ParseType
	{
		typedef Catalogy<0> NoneTypeCatalogy; 
		typedef Catalogy<1> BaseTypeCatalogy;
		typedef Catalogy<2> ComposeTypeCatalogy;
	}

	//define basetype
	template<typename T> struct ParseCatalogy { typedef  ParseType::NoneTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<bool>    { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<float>   { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<double>  { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<uint8>   { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<uint16>  { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<uint32>  { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<uint64>  { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<int8>    { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<int16>   { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<int32>   { typedef  ParseType::BaseTypeCatalogy catalogy; };
	template<>  struct ParseCatalogy<int64>   { typedef  ParseType::BaseTypeCatalogy catalogy; };

	//charset size equal
	template<typename charset1, typename charset2>
	struct IsCharsetCompatible{ enum { value = (sizeof(charset1) == sizeof(charset2)); }; };
	//To charset size must bigger than From charset
	template<typename From, typename To>
	struct IsCharsetConversible { enum { value = (sizeof(To) > sizeof(From)); }; };

	template<typename Select>
	struct LiteralBase
	{
		A3D_FORCEINLINE static CONSTEXPR achar  Select(const achar ansi, const wchar){ return ansi; }
		A3D_FORCEINLINE static CONSTEXPR achar* Select(const achar* ansi, const wchar*){ return ansi; }
	};

	template<>
	struct LiteralBase<wchar>
	{
		A3D_FORCEINLINE static CONSTEXPR wchar  Select(const achar, const wchar wch){ return wch; }
		A3D_FORCEINLINE static CONSTEXPR wchar* Select(const achar*, const wchar* wch){ return wch; }
	};


#define LITERIAL(ch, val) LiteralBase<ch>::Select(val, L##val)

}