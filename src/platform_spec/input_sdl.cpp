#include<sdl_events.h>
#include<sdl.h>
#include"input_sdl.h"
#include"window_sdl.h"
#include"../event/input_evt.h"
#include"../io/logger.h"

namespace Aurora3D
{
	int ConvertKeyCode(int key, int scancode)
	{
		if (SDL_SCANCODE_AC_BACK == scancode)
			return SDLK_ESCAPE;
		else return SDL_toupper(key);
	}
	Input::Input()
	{
		Initialize();
	}

	void Input::Initialize()
	{
		//todo
	}

	void Input::Update()
	{
		SDL_Event evt;
		while (SDL_PollEvent(&evt))
			HandleEvent(&evt);
	}


	void Input::HandleEvent(void* evt)
	{
		SDL_Event* sdl_evt = reinterpret_cast<SDL_Event*>(evt);
		switch (sdl_evt->type)
		{
		case SDL_QUIT:
			A3D_SEND_EVENT(CommonEvt::Quit, nullptr);
			break;
		case SDL_WINDOWEVENT:
			SetWindowReleated(sdl_evt->window.type);
			A3D_DEBUGGER("window event");
			break;
		case SDL_KEYDOWN:
			SetKey(sdl_evt->key.keysym.sym, sdl_evt->key.keysym.scancode, true);
			A3D_DEBUGGER("key pressed");
			break;
		case SDL_KEYUP:
			SetKey(sdl_evt->key.keysym.sym, sdl_evt->key.keysym.scancode, false);
			A3D_DEBUGGER("key released");
			break;
		case SDL_MOUSEBUTTONDOWN:
			SetMouse(sdl_evt->button.button, sdl_evt->button.clicks, true);
			A3D_DEBUGGER("mouse pressed");
			break;
		case SDL_MOUSEBUTTONUP:
			SetMouse(sdl_evt->button.button, sdl_evt->button.clicks, false);
			A3D_DEBUGGER("mouse released");
			break;
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
		default:
			break;
		}
	}

	void Input::SetWindowReleated(int evt_type)
	{
		std::shared_ptr<InputEvt::WindowReleatedData> data{
			new InputEvt::WindowReleatedData };
		data->evt_type = evt_type;
		A3D_SEND_EVENT(InputEvt::WindowReleated, data);
	}

	void Input::SetKey(int key, int scancode,  bool released)
	{
		bool repeat = false;
		key = ConvertKeyCode(key, scancode);
		if (!released)
		{
			scancode_down_.emplace(scancode);
			scancode_press_.emplace(scancode);
			if (key_down_.end() == key_down_.find(key))
			{
				key_down_.emplace(key);
				key_press_.emplace(key);
			}
			else{
				repeat = true;
			}
		}
		else{
			scancode_down_.erase(key);
			if (0 == key_down_.erase(key))
				return;
		}

		std::shared_ptr<InputEvt::KeyEventData> data{
			new InputEvt::KeyEventData };
		data->key = key;
		data->repeat = repeat;
		data->scancode = scancode;
		data->qualifier = GetQualifierKeys();
		A3D_SEND_EVENT(released ? InputEvt::KeyReleased : InputEvt::KeyPressed, data);
	}


	void Input::SetMouse(int key, int times, bool released)
	{
		if (!released)
		{
			if (!(mouse_down_ & key))
				mouse_pressed_ |= key;
			mouse_down_ |= key;
		}
		else{
			if (mouse_down_ & key)
				mouse_down_ &= ~key;
			else return;
		}
		std::shared_ptr<InputEvt::MouseEventData> mouse_evt{
			new InputEvt::MouseEventData };
		mouse_evt->cur_key = key;
		mouse_evt->times = times;
		mouse_evt->keys = mouse_pressed_;
		mouse_evt->qualifiter = GetQualifierKeys();
		A3D_SEND_EVENT(released ? InputEvt::MouseReleased : InputEvt::MousePressed, mouse_evt);
	}

	void Input::ResetState()
	{

	}

	bool Input::IsKeyPressed(int key)
	{
		return key_press_.end() != key_press_.find(SDL_tolower(key));
	}

	bool Input::IsKeyDown(int key)
	{
		return key_down_.end() != key_down_.find(SDL_tolower(key));
	}

	bool Input::IsQualifierDown(int qualifier)
	{
		if (KeyQualifier::Alt == qualifier)
			return IsKeyDown(SDLK_LALT) | IsKeyDown(SDLK_RALT);
		if (KeyQualifier::Ctrl == qualifier)
			return IsKeyDown(SDLK_LCTRL) | IsKeyDown(SDLK_RCTRL);
		if (KeyQualifier::Shift == qualifier)
			return IsKeyDown(SDLK_LSHIFT) | IsKeyDown(SDLK_RSHIFT);
		return false;
	}

	int Input::GetQualifierKeys() const
	{
		int qualifier = 0;
		for (auto q : { KeyQualifier::Alt,
			KeyQualifier::Shift, KeyQualifier::Ctrl })
		{
			qualifier |= q;
		}
		return qualifier;
	}
	
}