#pragma once

#include<memory>
#include<unordered_map>
#include<string>
#include<vector>
#include<functional>
#include<set>
#include"pulser.h"

//pre-defined header
#include"../common_def.h"
#include"../design/singleton.h"
#include"../event/event_def.h"
#include"../graphics/graphics_def.h"
#include"../platform_spec/platform_def.h"

namespace Aurora3D
{
	class Object;
	class Logger;
	class Graphics;
	class Application;

	class Engine:public Singleton<Engine>
	{
		using StringPairMap = std::unordered_map<std::string, std::string>;
		using ApplicationPtr = std::shared_ptr<Application>;
	private:
		GraphicsPtr graphics_ = nullptr;
		WindowPtr window_ = nullptr;
		ApplicationPtr app_ = nullptr;
		InputPtr input_ = nullptr;
		std::string exe_name_{};
		bool config_changed = true;
		Status status_{};
		bool running = true;
		StringPairMap parameters_{};
		Pulser pulser{};

		void Quit(EventData d);
	public:
		Engine(Application* app);

		//run once before start, parse cmd parameter
		void ProcessCmd(int argc, char* argv[]);

		//run once before engine loop
		bool Initialize();
		//every frame config before run
		void Config();
		//engine loop
		void Run();
		//exit recycle resource
		int Exit();

		void OnFatalErrorOccured(EventData data);
		
		void SetStatus(unsigned code, const char* msg){
			status_.code = code; status_.msg = msg;
		}
	};
}