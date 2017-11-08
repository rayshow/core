#pragma once

#include<cassert>
#include<memory>

namespace Aurora3D
{
	template<class T>
	class Singleton
	{
	public:
		Singleton(const Singleton<T> &) = delete;
		Singleton& operator=(const Singleton<T> &) = delete;

		Singleton()  
		{
			s_Singleton = static_cast< T* >(this);
		}

		static T* Initialize()
		{
			return new T{};
		}

		static void Destory()
		{
			if (s_Singleton)
			{
				delete s_Singleton;
				s_Singleton = nullptr;
			}
		}

		static  T* GetSingletonPtr()
		{
			assert(nullptr != s_Singleton);
			return s_Singleton;
		}

		static T& GetSingleton()
		{
			assert(nullptr != s_Singleton);
			return *s_Singleton;
		}
	public:
		static T *s_Singleton;
	};

	template<class T> T* Singleton<T>::s_Singleton = nullptr;
}