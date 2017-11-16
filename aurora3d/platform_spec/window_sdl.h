#pragma once
#include<SDL.h>
#include<string>
#include<vector>
#include<memory>
#include"../resource/resource_def.h"
#include"../design/singleton.h"
#include"../event/event_def.h"

namespace Aurora3D
{
	struct Resolution
	{
		unsigned w;
		unsigned h;
		bool operator==(const Resolution& r){
			return w == r.w && h == r.h;
		}
	};

	class Window
	{
	private:
		SDL_Window* window_ = nullptr;
		std::string title_ = "";
		ImagePtr icon_;
		unsigned x_ = 0;
		unsigned y_ = 0;
		unsigned width_ = 0;
		unsigned height_ = 0;
		bool fullscreen_ = false;
		bool input_focus_ = false;
		bool minimized_ = false;
		bool maximized_ = false;

		void* GetWindowHandle();
		void  OnWindowChangeEvent(EventData data);
		void OnResize();
		void OnMove();
		void GetDesktopResolutions(std::vector<Resolution>& rs);
		void FindMatchResolution(unsigned& width, unsigned& height);
	public:
		void Update();
		bool OpenWindow(unsigned width, unsigned height, bool fullscreen);
		void SetWindowTitle(const std::string& title);
		void SetOrientation(const std::string& ori);
		void SetPosition(unsigned x, unsigned y);
		void SetResolution(unsigned width, unsigned height, bool fullscreen);
		void SetIconImage(ImagePtr icon){}
		void SetInputFocus(bool enable){ input_focus_ = enable;  }
		bool ToggleFullscreen();
		void CloseWindow();
		bool IsWindowInputFocus(){ return input_focus_;  }
	};
}