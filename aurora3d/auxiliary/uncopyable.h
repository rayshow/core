#pragma once


namespace Aurora3D
{
	class Uncopyable
	{
	public:
		Uncopyable(){}
		~Uncopyable(){}
		Uncopyable(const Uncopyable& o) = delete;
		Uncopyable& operator=(const Uncopyable& o) = delete;
	};

}