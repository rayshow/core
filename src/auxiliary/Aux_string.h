#pragma once

#include<unordered_map>
#include<cstdarg>
#include<cstdio>
#include<cassert>
#include<tuple>
#include<memory>
#include<stack>
#include"../common_def.h"
#include"../platform_spec/GenericString.h"
#include"../platform_spec/GenericMemory.h"
#include"aux_string_trait.h"
#include"uncopyable.h"
#pragma warning(disable : 4996) 

namespace Aurora3D
{
	//formater buffer size config
	CONSTEXPR uint32 kFormaterDefaultBufferSize = 512;
	CONSTEXPR uint32 kFormaterMinimumBufferSize = 16;

	typedef std::vector<Range> RangeVec;
	typedef std::unordered_map<AString, AString> AStringMap;
	typedef std::vector<AString> AStringVec;
	typedef std::unordered_map<WString, WString> WStringMap;
	typedef std::vector<WString> WStringVec;

	//[begin, end) range of array
	struct Range
	{
		size_type begin;
		size_type end;
		size_type Length() const { return end - begin; }
		CCDK_FORCEINLINE bool IsValid() const { return Length()>0; }
		CCDK_FORCEINLINE Range(size_type Begin, size_type End):
			begin(Begin), end(End){}
		CCDK_FORCEINLINE Range(const Range& range) = default;
	};

	//common string operation
	struct StringRangeOp
	{
	public:
		template<typename Ch>
		inline static void Copy(
			Ch*   dest,
			const Ch*  src,
			size_type src_begin,
			size_type src_end)
		{
			assert(nullptr != dest && nullptr != src);
			const Ch* const CanNotReach = src + src_end;
			for (const Ch* it = src+src_begin ; it < CanNotReach; ++it, ++dest)
				*dest = *it;
		}

		template<>
		CCDK_FORCENOINLINE static void Copy<achar>(
			achar* dest,
			const achar* src,
			size_type src_begin,
			size_type src_end)
		{
			if (src_end>src_begin)
				PlatformMemory::Copy(dest, src+src_begin, src_end-src_begin);
		}

		template<typename Ch>
		CCDK_FORCEINLINE static Ch* AllocCopy(
			const Ch* src,
			size_type begin,
			size_type end)
		{
			assert(nullptr != src);
			size_type len = end - begin;
			Ch* dest = new Ch[len + 1];
			if (!dest) return nullptr;
			Copy(dest, src, begin, end);
			dest[len] = LITERIAL(Ch, '\0');
			return dest;
		}

		// return true if src[src_begin, src_end) and seq[ seq_begin, end) equals 
		// otherwise return false
		template<typename Ch>
		static bool Equals(
			const Ch* src,
			size_type src_begin,
			size_type src_end,
			const Ch* seq,
			size_type seq_begin)
		{
			assert(nullptr != src);
			assert(nullptr != seq);

			const Ch* const CanNotReach = src+src_end;
			const Ch* it = src + src_begin;
			for (; it < CanNotReach && (*it == *seq); ++it, ++seq);
			if (it >= CanNotReach) return true;
			return false;
		}

		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		inline static size_type FindFirst(
			const Ch* src,
			size_type begin,
			size_type end,
			size_type invalid,
			const Condi& pred)
		{
			assert(nullptr != src);
			const Ch* it = src+begin;
			const Ch* const CanNotReach = src + end;
			for (; it < CanNotReach &&!pred(*it); ++it);
			if (it >= CanNotReach) return invalid;
			return it - src;
		}


		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		CCDK_FORCEINLINE static size_type FindFirst(
			const Ch* src,
			size_type begin,
			size_type end,
			size_type invalid,
			Ch c)
		{
			static Condi find_char = [](Ch it){
				if (it == c) return true;
				return false;
			};
			return FindFirst(src, begin, end, invalid, find_char);
		}

		// 0  1  2  3  4  5  6  7  8   9
		// a  b  c  e  f  a  b  c  f   d
		//-1 -1 -1 -1 -1  0  1  2 -1  -1
		template<typename Ch>
		static void _built_prefix_from_front(
			const Ch* seq,
			size_type seq_len,
			int32* next)
		{
			int32 n = -1;
			next[0] = n;
			for (int32 i = 1; i < seq_len; ++i)
			{
				while (n > -1 && seq[n + 1] != seq[i]) n = next[n];
				if (seq[i] == seq[n + 1]) ++n;
				next[i] = n;
			}
		}


		// a b c a b c
		// 2 1 0-1-1-1
		template<typename Ch>
		static void _built_prefix_from_back(
			const Ch* seq,
			size_type seq_len,
			int32* next)
		{
			int32 n = -1;
			next[seq_len - 1] = -1;
			int32 cur = 0;
			for (int32 i = seq_len - 2; i >= 0; --i)
			{
				cur = seq - n - 1;
				while (n > -1 && seq[cur - 1] == seq[i]) n = next[cur];
				if (seq[cur - 1] == seq[i]) ++n;
				next[i] = n;
			}
		}

		//"------abcef----abcefabc-----abcefabcfd-----"
		// abcefabcfd
		//       abcefabcfd
		//       01234!->-1
		//                abcefabcfd
		//                01234567!->2
		//                     abcefabcfd
		//                        !->-1
		//                             abcefabcfd  seq_len = 10
		//                             0123456789  return i-9
		template<typename Ch>
		static size_type _kmp_find_from_front(
			const Ch* src,
			size_type src_begin,
			size_type src_end,
			size_type invalid,
			const Ch* seq,
			size_type seq_len,
			int32 *next)
		{
			int32 n = -1;
			for (int32 i = src_begin; i < src_end; ++i)
			{
				while (n > -1 && seq[n + 1] != src[i]) n = next[n];
				if (src[i] == seq[n + 1]) ++n;
				if (n == seq_len - 1) return i - n;
			}
			return invalid;
		}


		// -----abcabc---------dabc---
		//                      abcabc
		//                   abcabc
		//                   <-!210
		                        
		template<typename Ch>
		static size_type _kmp_find_from_back(
			const Ch* src,
			size_type src_begin,
			size_type src_end,
			size_type invalid,
			const Ch* seq,
			size_type seq_len,
			int32 *next)
		{
			int32 n = -1;
			for (int32 i = src_end-1; i >=src_begin; --i)
			{
				int32 cur = seq_len - n - 1;
				while (n > -1 && seq[cur-1] != src[i]) n = next[cur];
				if (src[i] == seq[cur-1]) ++n;
				if (n == seq_len - 1) return i;
			}
			return invalid;
		}


		template<typename Ch>
		static size_type FindFirst(
			const Ch* src,
			size_type src_begin,
			size_type src_end,
			size_type invalid,
			const Ch* seq,
			size_type seq_len)
		{
			assert(nullptr != src && nullptr != seq);

			if (seq_len == 0) return invalid; 
			diff_type off = (src_end - src_begin) - seq_len;
			if (off < 0) return invalid;        //out of range
			if (0 == off)
			{
				if (Equals(src,src_begin,src_end,seq,0)) return 0;  //completely equals
				else return invalid;
			}

			//KMP-algorithm
			std::unique_ptr<int32[]> next{ new int32[seq_len] };
			if (!next) return invalid;
			_built_prefix_from_front(seq, seq_len, next.get());
			return _kmp_find_from_front(src, src_begin, src_end, invalid, seq, seq_len, next.get());
		}

		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		inline static size_type FindLast(
			const Ch* src,
			size_type begin,
			size_type invalid,
			size_type end,
			const Condi& condi)
		{
			assert(nullptr != src);

			const Ch* it = src + end - 1;
			const Ch* const CanNotRearch = src+begin-1; 
			for (; it < CanNotRearch && !condi(*it); --it);
			if (it >= CanNotRearch) return invalid;
			return it - src; 
		}

		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		CCDK_FORCEINLINE static size_type FindLast(
			const Ch* src,
			size_type begin,
			size_type end,
			Ch c)
		{
			static Condi find_char = [](Ch it){
				if (it == c) return true;
				return false;
			};
			return FindLast(src, begin, end, find_char);
		}

		template<typename Ch>
		static size_type FindLast(
			const Ch* src,
			size_type src_begin,
			size_type src_end,
			size_type invalid,
			const Ch* seq,
			size_type seq_len)
		{
			assert(nullptr != src && nullptr != seq);
			if (0 == seq_len) return invalid;

			diff_type off = (src_end - src_begin) - seq_len;
			if (off < 0) return invalid;        //out of range
			if (0 == off)
			{
				if (Equals(src, src_begin, src_end, seq, 0)) return 0;  //completely equals
				else return invalid;
			}

			//KMP-algorithm
			std::unique_ptr<int32[]> next{ new int32[seq_len] };
			if (!next) return invalid;
			_built_prefix_from_back(seq, seq_len, next.get());
			return _kmp_find_from_back(src, src_begin, src_end, invalid, seq, seq_len, next.get());
		}

		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		static Range FindPair(const Ch* src,
			size_type begin,
			size_type end,
			const Condi& begin_condi,
			const Condi& end_condi)
		{
			assert(nullptr != src);
			const Ch* it1 = src+begin;
			const Ch* it2 = src + end - 1;
			while (it2 > it1 && !end_condi(*(it2))) --it2;   //find last march char's next pos
			while (it2 > it1 && !begin_condi(*it1)) ++it1;     //find first march char's pos
			if (it2 <= it1) return Range{};
			return Range{ it1 - src, it2 - src };
		}

		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		CCDK_FORCEINLINE static Range FindPair(
			const Ch* src,
			size_type begin,
			size_type end,
			Ch begin_sign,
			Ch end_sign)
		{
			static Condi find_begin = [](Ch c){
				if (c == begin_sign) return true;
				return false;
			};
			static Condi find_end = [](Ch c){
				if (c == end_sign) return true;
				return false;
			};
			return FindPair(src, begin, end, find_begin, find_end);
		}


		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		CCDK_FORCEINLINE static Range FindPair(
			const Ch* src,
			size_type src_begin,
			size_type src_end,
			Ch* begin_seq,
			size_type begin_seq_len,
			Ch* end_seq,
			size_type end_seq_len)
		{
			size_type begin_seq_start = FindFirst(src, src_begin, src_end, src_end,
				begin_seq, begin_seq_len);
			if (begin_seq_start == src_end) return Range{};
			size_type begin_seq_end = begin_seq_start + begin_seq_len;
			size_type end_seq_start = FindFirst(src, begin_seq_end, src_end, src_end,
				end_seq, end_seq_len);
			return Range{ begin_seq_end, end_seq_start };
		}

		//find non-space-tab range from two side
		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		CCDK_FORCEINLINE static Range Trim(
			const Ch* src,
			size_type begin,
			size_type end)
		{
			static Condi skip_space = [](Ch ch){
				if (LITERIAL(Ch, ' ') != ch && LITERIAL(Ch, '\t') != ch) return true;
				return false;
			};
			return FindPair(src, begin, end, skip_space, skip_space);
		}

		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		static bool Replace(Ch* src,
			const size_type begin,
			const size_type end,
			const Condi& condi,
			Ch replace)
		{
			assert(src != nullptr);
			assert(end>begin)

			bool find = false;
			const Ch* const CanNotReach = src + end;
			for (Ch* it = src+begin; it < CanNotReach; ++it)
				if (condi(*it))
				{
					find = true;
					*it = to;
				}
			return find;
		}

		template<typename Ch, typename SinglePredict = Char1stAppearCondi<Ch>::type>
		CCDK_FORCEINLINE static bool Replace(Ch* src,
			size_type begin,
			size_type end,
			Ch from,
			Ch replace)
		{
			static SinglePredict find_replace = [](Ch c){
				if (c == from) return true;
				return false;
			};
			return Replace(src, begin, end, find_replace, replace);
		}

		//search sign in range [begin, end),and return split range[begin, end)
		//" 1,2,3,4 " split by ',' get _1 2 3 4_
		//",1,2,3," get 1 2 3
		//",,1,2,3" get 1 2 3
		//"1,2,3,,,"  get 1 2 3
		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		static std::vector<size_type> Split(const Ch* src,
			size_type begin,
			size_type end,
			const Condi& condi)
		{
			assert(nullptr != src);

			std::vector<size_type> vec;
			const Ch* const CanNotReach = src + end;
			for (const Ch* it = src+begin; it<CanNotReach; ++it)
			{
				if (condi(*it))
				{
					vec.push_back(it - src);
				}
			}
			return vec;
		}

		template<typename Ch, typename Condi = Char1stAppearCondi<Ch>::type>
		CCDK_FORCEINLINE static std::vector<size_type> Split(
			const Ch* src,
			size_type begin,
			size_type end,
			Ch split_sign)
		{
			assert(nullptr != src);

			std::vector<size_type> vec;
			const Ch* const CanNotReach = src + end;
			for (const Ch* it = src + begin; it<CanNotReach; ++it)
			{
				if (split_sign == *it )
				{
					vec.push_back(it - src);
				}
			}
			return vec;
		}

		//KMP-find all match split-string and return non-split-string ranges
		template<typename Ch> 
		CCDK_FORCEINLINE static std::vector<size_type> Split(const Ch* src,
			size_type src_begin,
			size_type src_end,
			const Ch* seq,
			size_type seq_len)
		{
			assert(nullptr != src&& nullptr != seq);
			if (0 == seq_len) return src_end;

			std::vector<size_type> vec;
			std::unique_ptr<int32[]> next{ new int32[seq_len] };
			_built_prefix_from_front(seq, seq_len, next.get() );
			for (size_type i = src_begin; i != src_end;)
			{
				i = _kmp_find_from_front(src, i, src_end, src_end, seq, seq_len, next.get());
				vec.push_back(i);
			}
			return vec;
		};
	};

	struct StringConverter
	{
		template<typename From, typename To>
		static  std::enable_if_t< IsCharsetCompatible<To, From>::value, To*>
			Convert(To* dest, size_type destSize, const From* src, size_type srcSize)
		{
			if (destSize < srcSize) return nullptr;
			return PlatformMemory::Copy(dest, src, srcSize);
		}

		template<typename From, typename To>
		static  std::enable_if_t<IsCharsetConversible<To, From>::value, To*>
			Convert(To* dest, size_type destSize, const From* src, size_type srcSize)
		{
			const From* const* CanNotReach = src + srcSize;
			while (src && dest && src != CanNotReach)
			{
				*dest = (To)*src;
				++dest;
				++src;
			}
			return dest;
		}
	};

	//string formater with special stack BufferSize
	template<uint32 kFormaterBufferSize>
	class StringFormater
	{
	public:
		template<typename Ch, typename Container = CharContainerType<Ch>::type>
		static std::enable_if_t<IsSupportedCharset<Ch>::value, Container>
		Format(const Ch* fmt, prefix,...)
		{
			static_assert(kFormaterBufferSize >= kFormaterMinimumBufferSize, "Format Buffer Size too small");
			Ch buffer[kFormaterBufferSize];
			const uint32 count = kFormaterBufferSize - 1;
			va_list ap;
			va_begin(ap, fmt);
			uint32 result = PlatformString::FormatVariant(buffer, count, fmt, ap);
			va_end(ap);
			if (result > count) return Container{};
			buffer[result] = LITERIAL(Ch, '\0');
			return Container{ buffer };
		}
	};

	//string formater with default buffer size
	template<>
	class StringFormater<0>
	{
	public:
		template<typename Ch, typename Container = CharContainerType<Ch>::type>
		static std::enable_if_t<IsSupportedCharset<Ch>::value, Container>
			Format(const Ch* fmt, prefix,...)
		{
			
		}
	};

	//struct StringParser
	//{
	//private:
	//	template<typename Ch>
	//	static bool ParseToRanges(const Ch* src, 
	//		size_type src_len,
	//		const Ch* pattern,
	//		size_type pattern_len,
	//		RangeVec& src_ranges)
	//	{
	//		RangeVec pattern_ranges;
	//	}
	//public:
	//	template<typename Ch, typename T>
	//	static const Ch* ParsePrefix(const Ch* src, const Ch* prefix, T& t, bool IsBaseType)
	//	{
	//		assert(nullptr != src);
	//		assert(nullptr != prefix);
	//		Ch* parse_pos = PlatformString::FindSequence(src, prefix);
	//		if (parse_pos)
	//		{
	//			size_type len = PlatformString::Length(src);
	//			parse_pos += len;
	//			if (IsBaseType)
	//				return PlatformString::FromString(parse_pos, t);
	//			else{
	//				return t.FromString(parse_pos);
	//			}
	//		}
	//		return nullptr;
	//	}

	//	//"{"id":5, "name":"xiongya","addr":{"stress":"190"} }"
	//	//"{"id":(?), "name":(?),"addr":(?) }"
	//	//"[(?),(?),(?),(?)]"  array
	//	//"{(?),(?),(?),(?)}"  object
	//	template<typename Ch, typename prefix,...Args>
	//	static const Ch* ParsePattern(const Ch* src, const Ch* pattern, Argsprefix,... args)
	//	{
	//		//StringRangeOp::FindPair(src,0,)

	//		return nullptr;
	//	}
	//};


#define STRING_OBJECT_START '{'
#define STRING_OBJECT_END '}'
#define STRING_ARRAY_START '['
#define STRING_ARRAY_END ']'
#define STRING_PLACE_SEQUENCE "(?)"


	//stack memory string opeation
	template<typename Container, typename CharType = StringBaseType<Container>::type>
	class AuxString :public Uncopyable
	{
	private:
		const CharType*  ref = nullptr;
		size_type        ref_length = 0;
		size_type        begin = 0;
		size_type        end = 0;
	public:
		typedef Char1stAppearCondi<CharType>::type FirstCondi;
		typedef AuxString<Container>                Self;
		typedef std::vector<Self>                  SplitResult;
		
		AuxString() = default;

		//shared with Other with same content
		AuxString(const Self& Oth) :
			ref(Oth.ref),
			ref_length(Oth.ref_length),
			begin(Oth.begin),
			end(Oth.end)
		{
		}

		AuxString(const Self& oth, Range& range) :
			ref(oth.ref),
			ref_length(oth.ref_length),
			begin(range.begin),
			end(range.end){}

		AuxString(const Self& oth, size_type Begin, size_type End) :
			ref(oth.ref),
			ref_length(oth.ref_length),
			begin(Begin),
			end(End){}

		//copy construct from AString,WString
		AuxString(const Container& ct)
		{
			if (!ct.empty())
			{
				ref = &ct[0];
				ref_length = ct.length();
				begin = 0;
				end = ref_length;
			}
		}

		~AuxString()
		{
			if (ref)
			{
				ref = nullptr;
				ref_length = 0;
				begin = end = 0;
			}
		}

		bool IsExists()
		{
			return begin < end && ref != nullptr;
		}


		bool operator bool()
		{
			return IsExists();
		}

		CharType operator[](size_type index)
		{
			return ref[index + begin];
		}
		
		const CharType operator[](size_type index) const
		{
			return ref[index + begin];
		}

		const CharType AtBegin(size_type index) const
		{
			return ref[index + begin];
		}

		const CharType AtEnd(size_type index) const
		{
			return ref[end - 1 - index];
		}


		Self Trim() const
		{
			return AuxString{ *this, StringRangeOp::Trim(ref, begin, end) };
		}

		Self BeforeFirstFind(CharType c) const
		{
			return AuxString{ *this, begin, StringRangeOp::FindFirst(ref, begin, end, begin, c) };
		}

		Self BeforeFirstFind(const Container& ct) const
		{
			return AuxString{ *this, begin, StringRangeOp::FindFirst(
				ref, begin, end, begin, ct.c_str(), ct.length()) };
		}

		Self BeforeLastFind(CharType c) const
		{
			return AuxString{ *this, begin, StringRangeOp::FindFirst(
				ref, begin, end, begin, ct.c_str(), ct.length()) };
		}

		Self BeforeLastFind(const Container& ct) const
		{
			return AuxString{ *this, begin, StringRangeOp::FindLast(
				ref, begin, end, begin, ct.c_str(), ct.length()) };
		}

		Self AfterFirstFind(CharType c) const
		{
			return AuxString{ *this, StringRangeOp::FindFirst(ref, begin, end, end, c) + 1, end };
		}

		//"--------(SEQ)--------" get back part of split string
		// result is    --------
		Self AfterFirstFind(const Container& ct) const
		{
			size_type pos = StringRangeOp::FindFirst(ref, begin, end, ct.c_str(), ct.length());
			return AuxString{ *this, pos + ct.length(), end };
		}


		//    "-----(------)-----" find a range in begin_sign and end_sign like ( and  ) 
		// result is ------
		Self InnerPair(CharType begin_sign, CharType end_sign) const
		{
			return AuxString{ *this, StringRangeOp::FindPair(ref, begin, end, begin_sign, end_sign) };
		}

		//"-----(BEGIN_SEQ)------(END_SEQ)-----" find a range in begin_seq and end_seq
		// result is       ------                     
		Self InnerPair(const Container& begin_seq, const Container& end_seq) const
		{
			return AuxString{ *this, StringRangeOp::FindPair(ref, begin, end, 
				begin_seq.c_str(), begin_seq.length(), end_seq.c_str(), end_seq.length()) };
		}
		 
		//"------s----s-----"  s is a split char, result is
		// ------|----|-----|  
		std::vector<Self> Split(CharType sign)
		{
			auto split_poses = StringRangeOp::Split(ref, begin, end, sign);
			if (split_poses.empty()) return std::vector<AuxString>{};

			size_type last = 0;
			std::vector<AuxString> aux_strings;
			for (size_type it :split_poses)
			{
				if ( last < it ) aux_strings.emplace_back(*this, last, it);
				last = it + 1; //new start
			}
			if (last < end) aux_strings.emplace_back(*this, last, end);
			return aux_strings;
		}

		//" { name:" jjii{}\" " }"
		std::vector<Self> SplitIngorePair(CharType sign, const CharType* ingore_begin, const CharType* ingore_end)
		{
			//char *p = "fdafda\" fdsaf\}"
			std::stack<CharType> ingore_pair;
			size_type len = PlatformString::Length(ingore_begin);
			const CharType* const CanNotReach = ref + end;
			const CharType* it = ref;
			while (it != CanNotReach)
			{
				size_type index = len;
				bool need_end = !ingore_pair.empty();  //found a sign of "{[(
			}
			for (size_type it = begin; it < end; ++it)
			{
				if ( len != (index = StringRangeOp::FindFirst(ingore_begin, 0, len, len, *it)))
				{
					ingore_pair.push(index);
				}
				else if (len != (index = StringRangeOp::FindFirst(ingore_end, 0, len, len, *it)))
				{
					//not pair
					if (ingore_pair.top() != index){
						return std::vector<Self>{};
					}
					
				}
			}
		}

		//"----(?)----(?)(?)-----" ,(?) is a split string sequence, result is
	    // ----|  ----|     -----|  
		std::vector<Self> Split(const Container& seq)
		{
			size_type seq_len = seq.length();
			auto split_poses = StringRangeOp::Split(ref, begin, end, seq.c_str(), seq_len);
			if (split_poses.empty()) return std::vector<AuxString>{};
			size_type last = 0;
			std::vector<AuxString> aux_strings;
			for (size_type it : split_poses)
			{
				//end < new_start
				if ( last < it ) aux_strings.emplace_back(*this, last, it);
				last = it+seq_len;
			}
			if (last < end)
			{
				aux_strings.emplace_back(*this, last, end);
			}
			return aux_strings;
		}

		
		template<size_type BufferSize>
		Container Format(const CharType *fmt, prefix,...)
		{
			static_assert(BufferSize >= kFormaterMinimumBufferSize, "Format Buffer Size too small");
			CharType buffer[BufferSize];
			const uint32 count = BufferSize - 1;
			va_list ap;
			va_start(ap, fmt);
			uint32 result = PlatformString::FormatVariant(buffer, count, fmt, ap);
			va_end(ap);
			if (result > count) return Container{};
			buffer[result] = LITERIAL(CharType, '\0');
			return Container{ buffer };
		}

		template<>
		Container Format<0>(const CharType* fmt, prefix,...)
		{
			CharType buffer[kFormaterDefaultBufferSize];
			const uint32 count = kFormaterDefaultBufferSize - 1;
			va_list ap;
			va_start(ap, fmt);
			uint32 result = PlatformString::FormatVariant(buffer, count, fmt, ap);
			va_end(ap);
			if (result > count) return Container{}; 
			buffer[result] = LITERIAL(CharType, '\0');
			return Container{ buffer };
		}
		 b 
		//src like "{name:"xiongya", age:27, address:{ street:"luoyu Road", building:"xxx" }, families:["aa","bb","cc"] }"
		//fmt like "{name:(?), arg:(?), address:(?), families:(?) }"
		template<typename prefix,...Args, size_type ArgsSize = sizeofprefix,...(Args)>
		const CharType* Parse(const AuxString& src, const AuxString& fmt, Argsprefix,... args)
		{
			AuxString src_trim = src.Trim();
			AuxString fmt_trim = fmt.Trim();
			//begin with '{', end with '}'
			if (!src_trim.IsExists() || !fmt_trim.IsExists() ||
				src_trim.AtBegin(0) != LITERIAL(CharType, STRING_OBJECT_START) ||
				src_trim.AtEnd(0) != LITERIAL(CharType, STRING_OBJECT_END) ||
				fmt_trim.AtBegin(0) != LITERIAL(CharType, STRING_OBJECT_START)||
				fmt_trim.AtEnd(0) != LITERIAL(CharType, STRING_OBJECT_END))
				return nullptr;

			AuxString src_content{ src_trim.ref, src_trim.begin + 1, src_trim.end - 1 };
			AuxString fmt_content{ fmt_trim.ref, fmt_trim.begin + 1, fmt_trim.end - 1 };
			if (!src_content.IsExists() || !fmt_content.IsExists()) return nullptr;
			//" name : (?) "
			std::vector<AuxString> split_results = src_content.Split(LITERIAL(CharType, ','));
			if (split_results.empty()) return nullptr;
			for (AuxString it : split_results)
			{
				std::vector<AuxString> pair = it.Split(':');
			}
		}


	};
}