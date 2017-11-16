#pragma once
#include<memory>
namespace Aurora3D
{
	class Window;
	class Input;
	typedef std::shared_ptr<Window> WindowPtr;
	typedef std::shared_ptr<Input> InputPtr;
}