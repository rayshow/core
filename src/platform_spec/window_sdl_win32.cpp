#pragma once
#include<sdl_syswm.h>
#include<memory>

#include"window_sdl.h"
#include"../precompile_def.h"


#if defined(AURORA3D_WINDOW)

#include"../io/logger.h"
#include"../event/window_evt.h"
#include"../event/input_evt.h"

#include<dxgiformat.h>
#include<numeric>

namespace Aurora3D
{
	void Window::SetWindowTitle(const std::string& title)
	{
		if (title_ != title)
		{
			title_ = title;
			if (window_) SDL_SetWindowTitle(window_, title.c_str());
		}
	}

	void* Window::GetWindowHandle()
	{
		SDL_SysWMinfo sysInfo;
		SDL_VERSION(&sysInfo.version);
		SDL_GetWindowWMInfo(window_, &sysInfo);
		return sysInfo.info.win.window;
	}

	void Window::SetOrientation(const std::string& ori)
	{
		SDL_SetHint(SDL_HINT_ORIENTATIONS, ori.c_str());
	}

	void Window::SetPosition(unsigned x, unsigned y)
	{
		if (x != x_ || y != y_)
		{
			x_ = x;
			y_ = y;
			if (window_) SDL_SetWindowPosition(window_, x, y);
		}
	}

	bool Window::OpenWindow(unsigned width, unsigned height, bool fullscreen)
	{
		window_ = SDL_CreateWindow("sdl window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
		if (!window_)
		{
			A3D_ERROR("create sdl window failed.");
			return false;
		}
		int x, y, w, h;
		SDL_GetWindowPosition(window_, &x, &y);
		SDL_GetWindowSize(window_, &w, &h);
		x_ = x;
		y_ = y;
		width_ = w;
		height_ = h;

		A3D_SUBSCRIBE_EVENT(InputEvt::WindowReleated, &Window::OnWindowChangeEvent);


		std::shared_ptr<WindowEvt::DisplayData> display_data{ new WindowEvt::DisplayData };
		display_data->width = width_;
		display_data->height = height_;
		display_data->window_handle = this->GetWindowHandle();
		A3D_SEND_EVENT(WindowEvt::WindowCreated, display_data);
		return true;
	}

	void Window::OnResize()
	{
		if (fullscreen_) return;

		int new_width, new_height;
		SDL_GetWindowSize(window_, &new_width, &new_height);
		std::shared_ptr<WindowEvt::DisplayData> display_data{
			new WindowEvt::DisplayData };

		if (new_width == width_ && new_height == height_)
			return;

		A3D_DEBUGGER("window resized");

		width_ = new_width;
		height_ = new_height;
		display_data->width = width_;
		display_data->height = height_;
		display_data->window_handle = this->GetWindowHandle();
		A3D_SEND_EVENT(WindowEvt::DisplayChanged, display_data);
	}

	void Window::OnMove()
	{
		if (fullscreen_) return;
		int new_x, new_y;
		SDL_GetWindowPosition(window_, &new_x, &new_y);
		if (x_ == new_x && y_ == new_y)
			return;
		x_ = new_x;
		y_ = new_y;
		A3D_DEBUGGER("window moved");
	}

	void Window::OnWindowChangeEvent(EventData data)
	{
		auto window_data = std::static_pointer_cast<InputEvt::WindowReleatedData>(data);
		switch (window_data->evt_type)
		{
		case SDL_WINDOWEVENT_MINIMIZED:
			minimized_ = true;
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
			minimized_ = false;
			break;
		case SDL_WINDOWEVENT_RESIZED:
			OnResize();
			break;
		case SDL_WINDOWEVENT_MOVED:
			OnMove();
			break;
		default:
			break;
		} 
	}

	void Window::SetResolution(unsigned width, unsigned height, bool fullscreen)
	{
		if (1 > width || 1 > height) return;
		if (width == width_ && height == height_ && fullscreen == fullscreen_)
			return;

		SDL_DisplayMode mode;
		SDL_GetDesktopDisplayMode(0, &mode);
		DXGI_FORMAT fullscreen_format = SDL_BITSPERPIXEL(mode.format) == 16 ? DXGI_FORMAT_B5G6R5_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;
		if (fullscreen)
		{
			FindMatchResolution(width, height);
		}
		SDL_SetWindowSize(window_, width, height);
		SDL_SetWindowFullscreen(window_, fullscreen ? SDL_TRUE : SDL_FALSE);
		int w, h;
		SDL_GetWindowSize(window_, &w, &h);
		width_ = w;
		height_ = h;

		std::shared_ptr<WindowEvt::DisplayData> display_data{
			new WindowEvt::DisplayData };
		display_data->width = width_;
		display_data->height = height_;
		display_data->window_handle = this->GetWindowHandle();
		A3D_SEND_EVENT(WindowEvt::DisplayChanged, display_data );
	}

	void Window::GetDesktopResolutions(std::vector<Resolution>& rs)
	{
		unsigned num = SDL_GetNumDisplayModes(0);
		for (unsigned i = 0; i < num; ++i)
		{
			SDL_DisplayMode mode;
			SDL_GetDisplayMode(0, i, &mode);
			Resolution r = { mode.w, mode.h };

			bool find_duplicated = false;
			for (auto& res:rs)
				if (res == r){
					find_duplicated = true;
					break;
				}
			if (find_duplicated) continue;
			rs.push_back(r);
		}
	}

	void Window::FindMatchResolution(unsigned& width, unsigned& height)
	{
		unsigned num = SDL_GetNumDisplayModes(0);

		unsigned best_err = std::numeric_limits<unsigned>::max();
		for (unsigned i = 0; i < num; ++i)
		{
			SDL_DisplayMode mode;
			SDL_GetDisplayMode(0, i, &mode);
			
			unsigned err = std::abs((int)width - mode.w) + std::abs((int)height - mode.h);
			if (0 == err) return;
			if (err < best_err)
			{
				width = mode.w;
				height = mode.h;
			}
		}
	}
}

#endif