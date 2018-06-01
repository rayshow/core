#pragma once

#include<memory>
#include<unordered_map>
#include<string>
#include<vector>
#include<functional>
#include<set>
#include<parser.h>

//pre-defined header
#include<ccdk/mpl/design_pattern/singleton.h>
#include<aurora3d/module.h>
#include<aurora3d/event/event_def.h>
#include<aurora3d/graphic/graphics_def.h>
#include<aurora3d/platform_spec/platform_def.h>

a3d_namespace_engine_start

using namespace ccdk::mpl;

class Object;
class Logger;
class Graphics;
class Application;

class Engine : public dp::singleton<Engine>
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

a3d_namespace_engine_end