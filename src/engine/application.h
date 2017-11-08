#pragma once
#include<memory>
#include<string>
#include"engine.h"

namespace Aurora3D
{
	class Application
	{
	private:
		std::string app_name_{};
	public:
		void SetApplicationName(const std::string& name) { app_name_ = name; }
		std::string GetApplicationName(){ return app_name_;  }
		virtual void Initialize(){}
		virtual void Config() {}
		virtual void Update() {}
		virtual void Exit() {}
		virtual ~Application(){}
	};
}

//window console and open memory leak detection
#if defined(AURORA3D_WINDOW) && defined(AURORA3D_DEBUG) 
#include<windows.h>
#include<crtdbg.h>
#define OpenConsoleAndDetechMemoryLeak() \
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
	AllocConsole(); \
	SetConsoleTitle("debug info:"); \
	FILE* consoleFile = nullptr; \
	freopen_s(&consoleFile, "CONOUT$", "wb", stdout); 


#define SetBreakPointAtMemoryLeak(addr) \
	_CrtSetBreakAlloc(addr);

#else


#define OpenConsoleAndDetechMemoryLeak 
#define SetBreakPointAtMemoryLeak
#endif



#define AURORA3D_MAIN_APPLICATION(app_class_name) \
int main(int argc, char** argv) { \
	OpenConsoleAndDetechMemoryLeak \
	Aurora3D::Application *app = new app_class_name{}; \
	Aurora3D::Engine engine(app); \
	engine.ProcessCmd(argc, argv); \
	engine.Run(); \
	return engine.Exit(); \
} 