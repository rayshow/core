//#pragma once
//
//#include"../common_def.h"
//#include"../string/astring.h"
//
//namespace Aurora3D
//{
//	class  IntVector2
//	{
//	public:
//		int32 x = 0;
//		int32 y = 0;
//		CCDK_FORCEINLINE const int32* GetBuffer() const { return &x; }
//	
//		///construct
//		IntVector2() = default;
//		CCDK_FORCEINLINE IntVector2(const IntVector2& o) :x(o.x), y(o.y){}
//		CCDK_FORCEINLINE explicit IntVector2(int ax, int ay) : x(ax), y(ay) {}
//		CCDK_FORCEINLINE explicit IntVector2(const int* data) : x(data[0]), y(data[1]){}
//		CCDK_FORCEINLINE IntVector2& operator =(const IntVector2& rhs) { x = rhs.x; y = rhs.y; return *this; }
//
//		///operation
//		CCDK_FORCEINLINE bool operator ==(const IntVector2& r) const { return x == r.x && y == r.y; }
//		CCDK_FORCEINLINE bool operator !=(const IntVector2& r) const { return x != r.x || y != r.y; }
//
//		CCDK_FORCEINLINE IntVector2 operator -() const { return IntVector2{ -x, -y }; }
//		CCDK_FORCEINLINE IntVector2 operator +(const IntVector2& r) const { IntVector2{ x + r.x, y + r.y }; }
//		CCDK_FORCEINLINE IntVector2 operator -(const IntVector2& r) const { IntVector2{ x - r.x, y - r.y }; }
//		CCDK_FORCEINLINE IntVector2 operator *(int I)  const { return IntVector2{ x*I, y*I }; }
//		CCDK_FORCEINLINE const IntVector2& operator +=(const IntVector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
//		CCDK_FORCEINLINE const IntVector2& operator -=(const IntVector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
//		CCDK_FORCEINLINE const IntVector2& operator *=(int rhs) { x *= rhs; y *= rhs; return *this; }
//		
//		CCDK_FORCEINLINE AString ToAString()
//		{
//			return StringFormater<32>::Format("[%d, %d]", x, y);
//		}
//
//		void FromAString(const AString& str)
//		{
//			
//		}
//
//
//	};
//
//	inline IntVector2 operator *(int l, const IntVector2& r) 
//	{ 
//		return IntVector2{ l* r.x, r.y * l };
//	}
//}
//
