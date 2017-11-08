
#include<iostream>
#include<stdio.h>

#include"engine.h"
#include"application.h"
#include"../platform_spec/window_sdl.h"
#include"../graphics/graphics.h"
#include"../io/logger.h"
#include"../io/file_system.h"
#include"../string/astring.h"
#include"../event/engine_evt.h"
#include"../platform_spec/input_sdl.h"

namespace Aurora3D
{
	Engine::Engine(Application* app)
	{
		if (nullptr == app)
		{
			SetStatus(Status::InitialFailed, "Application not allocated!");
			return;
		}
		app_.reset(app);
	}

	void Engine::ProcessCmd(int argc, char* argv[])
	{
		exe_name_ = PathHelper::UnixStylePath(argv[0]);
		if (argc >= 2)
		{
			//store parameter from cmd line
			for (int i = 1; i < argc; ++i)
			{
				/*std::string name, value;
				if (StringHelper::SplitToTwo(argv[i], '=', name, value))
				{
					parameters_.emplace(name, value);
				}
				else{
					SetStatus(Status::InitialFailed, ("comfigure parameter " + std::string(argv[i]) + " is error").c_str());
					return;
				}*/
			}

			//load configure parameter from file,will not cover cmd parameter and add new parameter from file
			std::string config_file{"Aurora3D.ini"};
			if (parameters_.find("ConfigureFile") != parameters_.end())
			{
				config_file = parameters_["ConfigureFile"];
				StringPairMap read_parameters;
				//if (StringHelper::ParseIniFile(config_file, read_parameters))
				//{
				//	for (auto& pair : read_parameters)
				//	{
				//		//not exists and update into global parameters
				//		if (parameters_.find(pair.first) == parameters_.end())
				//			parameters_.emplace(pair.first, pair.second);
				//	}
				//}
			}
		}
		
		//no log file specified
		if (parameters_.find("LogFileName") == parameters_.end())
		{
			if (app_->GetApplicationName() != "")
				parameters_["LogFileName"] = app_->GetApplicationName()+".log";
			else
				parameters_["LogFileName"] = exe_name_+".log";
		}
	}

	void Engine::Config()
	{
		app_->Config();
		if (config_changed)
		{
			IntVector2 resolution(800, 600);
			/*resolution.FromAString(StringHelper::GetString(parameters_, "Resolution", ""));

			int mutilsample = StringHelper::GetInt(parameters_, "MutilSample", 1);
			bool vsyn = StringHelper::GetBool(parameters_, "Vsync", false);
			bool fullscreen = StringHelper::GetBool(parameters_, "FullScreen", false);*/

			/*graphics_->SetVsync(vsyn);
			graphics_->SetMutilSample(mutilsample);
			window_->SetResolution(resolution.x+1,
				resolution.y, fullscreen);*/
			config_changed = false;
		}
	}

	//log system first to record what happended, then event system
	//to notify fatal event to recycle resource
	bool Engine::Initialize()
	{
		//initialize log system
		auto log = Logger::Initialize();
		if (!log || !log->Open(parameters_["LogFileName"]))
		{
			SetStatus(Status::InitialFailed, "log system initialize failed"); 
			return false;
		}
		A3D_INFO("log system initialized!");

		//initialize event system
		auto dispatcher = EventDispatcher::Initialize();
		if (!dispatcher)
		{
			SetStatus(Status::InitialFailed, "Event system initialize failed");
			return false;
		}
		A3D_INFO("event system initialized!");

		//create graphics system
		graphics_.reset(new Graphics());
		if (!graphics_)
		{
			SetStatus(Status::InitialFailed, "graphics system create error.");
			return false;
		}

		//create input system
		input_.reset(new Input());
		
		//open window
		window_.reset(new Window());
		if (!window_)
		{
			SetStatus(Status::InitialFailed, "window manager create error.");
			return false;
		}

		/*window_->SetOrientation(StringHelper::GetString(parameters_, "Orientation", "LandscapeLeft LandscapeRight"));*/

		//open window
		IntVector2 resolution(800, 600);
		/*resolution.FromAString(StringHelper::GetString(parameters_, "Resolution", ""));
		bool fullscreen = StringHelper::GetBool(parameters_, "FullScreen", false);*/
		//if (!window_->OpenWindow(resolution.x, resolution.y, fullscreen))
		//{
		//	SetStatus(Status::InitialFailed, "window create error.");
		//	return false;
		//}

		//subscribe to error event
		A3D_SUBSCRIBE_EVENT(CommonEvt::FatalError, &Engine::OnFatalErrorOccured);
		A3D_SUBSCRIBE_EVENT(CommonEvt::Quit, &Engine::Quit);
		return true;
	}

	void Engine::Run()
	{
		if (!Initialize()) return;
		while (status_.Ok() && running)
		{
			Config();
			pulser.Pulse();
			input_->Update();
			app_->Update();
			pulser.Sleep();
		}
	}

	void Engine::Quit(EventData data)
	{
		running = false;
	}
	int Engine::Exit()
	{
		if (!status_.Ok())
		{
			//with log system
			if (Logger::GetSingletonPtr())
				A3D_FATAL( "engine exit of fatal error" );
		}
		Logger::Destory();
		EventDispatcher::Destory();
		return status_.code;
	}


	void Engine::OnFatalErrorOccured(EventData data)
	{
		auto fatal_msg = std::static_pointer_cast<Status>(data);
		status_ = *fatal_msg;
	}

}