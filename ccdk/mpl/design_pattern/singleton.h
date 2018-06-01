#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_design_pattern_start

template<typename T>
class singleton
{
	using this_type     = singleton;

private:
	static T *s_instance;
public:
	singleton(this_type const&) = delete;
	singleton& operator=(this_type const&) = delete;

	singleton()
	{
		s_instance = static_cast< T*>(this);
	}

	static void initialize(){
		s_instance = new T{};
	}

	static void destory()
	{
		if (s_instance)
		{
			delete s_instance;
		}
	}

	static T* instance()
	{
		assert(nullptr != s_instance);
		return s_instance;
	}
};

template<typename T> T* singleton<T>::s_instance = nullptr;

ccdk_namespace_mpl_design_pattern_end