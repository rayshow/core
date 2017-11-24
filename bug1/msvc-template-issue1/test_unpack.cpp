#include<type_traits>
#include<string>

using namespace std;


#ifdef __GNUC__ 
// not defined ?
template<typename T, T... args>
struct integer_sequence
{
	//static constexpr unsigned int size = sizeof...(args);
	typedef integer_sequence<T, args...> type;
	typedef T value_type;
};
template<unsigned int... args> using index_sequence = integer_sequence<unsigned int, args...>;


template<unsigned int... args> using index_sequence = integer_sequence<unsigned int, args...>;

template<unsigned int n, unsigned int... args> struct make_index_sequence_t :make_index_sequence_t<n - 1, n - 1, args...> {};
template<unsigned int... args> struct make_index_sequence_t<0, args...> { typedef index_sequence< args...> type; };

//generate 0 .. n -1 sequence
template<unsigned int n>
using make_index_sequence = typename make_index_sequence_t<n>::type;

#endif

//is empty class, derive V
template<typename K, typename V, bool = !is_final<V>::value && is_empty<V>::value >
struct ebo: public V
{
	constexpr ebo() = default;

	//any type can be convert to V, call V constructor
	template<typename T>
	constexpr ebo(const T& inV) :V(inV) {}
};

//not class type or not empty class  or is final
template<typename K, typename V>
struct ebo<K, V, false>
{
	V v;

	constexpr ebo() = default;

	//V can use T construct
	template<typename T>
	constexpr ebo(const T& inV) : v{ inV } {}
};

//supply K, deduce V, and get ebo value
template<typename K, typename V> constexpr const auto& get_ebo(const ebo<K, V, true>& e) { return e; }
template<typename K, typename V> constexpr const auto& get_ebo(const ebo<K, V, false>& e) { return e.v; }

template<unsigned int i>
using idx = integral_constant<unsigned int, i>;

template< typename ,typename...Args>
struct sp_tuple_t {};


//derive all Args 
template<  unsigned int... indice,  typename... Args>
struct sp_tuple_t< index_sequence<indice...>, Args...>:
	ebo< idx<indice>, Args>...
{
	static constexpr int length = sizeof...(Args);

	//just copy it and initalize base class in order
	constexpr sp_tuple_t(Args... args) : ebo< idx<indice>, Args>( std::move(args) )... {}

	//merge two tuple
	template<unsigned int... indice1, unsigned int... indice2, typename... Args1, typename... Args2, typename T1, typename T2>
	constexpr sp_tuple_t(  index_sequence<indice1...>,
		                   index_sequence<indice2...>,
						   const sp_tuple_t< T1, Args1...>& t1,
						   const sp_tuple_t< T2, Args2...>& t2)
		  : ebo< idx<indice1>, Args1>(t1[idx<indice1>{}])..., ebo< idx<t1.length + indice2>, Args2>(t2[idx<indice2>{}])...  //compile failed, interal error 
		//: ebo< idx<indice1>, Args1>(t1[idx<indice1>{}])... //will be compile ok
		//: ebo< idx<t1.length + indice2>, Args2>(t2[idx<indice2>{}])... //will be compile ok
	{

	}

	//read only
	template<unsigned int i>
	constexpr const auto operator[](idx<i>) const
	{
		return get_ebo<idx<i>>(*this);
	}

	//merge two tuple
	template<typename... Args1, typename T>
	constexpr auto operator+(const sp_tuple_t< T, Args1...>& t1) const
	{
		using new_tuple = sp_tuple_t < make_index_sequence<length + t1.length>, Args..., Args1...>;
		return new_tuple{ index_sequence<indice...>{},  make_index_sequence<t1.length>{}, *this, t1 };
		
	}
};

template<typename... Args>
using sp_tuple = sp_tuple_t< make_index_sequence< sizeof...(Args) >, Args...>;

int main()
{
	constexpr sp_tuple<char, int, float> tp1('a', 2, 1.3f);
	static_assert(tp1[idx<0u>{}] == 'a', "");
	static_assert(tp1[idx<1u>{}] == 2, "");
	static_assert(tp1[idx<2u>{}] == 1.3f, "");

	constexpr sp_tuple<char,char> tp2('b','c');
	static_assert(tp2[idx<0u>{}] == 'b', "");
	static_assert(tp2[idx<1u>{}] == 'c', "");

	//bug for msvc2017, clang gcc6 is ok for this
	//see line 57-59, only uncommit 58 or 59 will compile ok
	constexpr auto tp3 = tp1 + tp2;
	static_assert(tp3[idx<0>{}] == 'a', "");
	static_assert(tp3[idx<1>{}] == 2, "");
	static_assert(tp3[idx<2>{}] == 1.3f, "");
	static_assert(tp3[idx<3>{}] == 'b', "");
	static_assert(tp3[idx<4>{}] == 'c', "");
	
	getchar();
}