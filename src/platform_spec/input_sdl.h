#pragma once

#include<unordered_set>

#include"../design/singleton.h"
#include"../resource/resource_def.h"
#include"platform_def.h"

namespace Aurora3D
{
	struct KeyQualifier
	{
		const static unsigned Ctrl = 1;
		const static unsigned Alt = 2;
		const static unsigned Shift = 4;
	};

	class Input
	{
	private:
		std::weak_ptr<Window> window_{};
		std::unordered_set<int> key_down_{};
		std::unordered_set<int> key_press_{};
		std::unordered_set<int> scancode_down_{};
		std::unordered_set<int> scancode_press_{};
		bool mouse_visible_ = false;
		bool input_focus_ = false;
		unsigned mouse_down_ = 0;
		unsigned mouse_pressed_ = 0;

		void Initialize();
		void HandleEvent(void* evt);
		void ResetState();
		void SetKey(int key, int scancode,bool released);
		void SetMouse(int key, int times, bool released);
		void SetWindowReleated(int evt_type);
	public:
		Input();
		Input(WindowPtr w) :window_(w){};
		~Input(){}
		void Update();
		void SetMouseGrab(bool grab);
		void SetMouseVisible(bool visible);
		void SetCursorImage(ImagePtr img);
		bool IsKeyPressed(int key);
		bool IsKeyDown(int key);
		bool IsQualifierDown(int qualifier);
		int  GetQualifierKeys() const;
	};
}