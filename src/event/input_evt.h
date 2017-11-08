#pragma once
#include"../common_def.h"
#include"event_dispatcher.h"

namespace Aurora3D
{
	struct InputEvt
	{
		const static EventID KeyPressed = ID(A3DModule::Input, 1);
		const static EventID KeyReleased = ID(A3DModule::Input, 2);
		const static EventID MousePressed = ID(A3DModule::Input, 3);
		const static EventID MouseReleased = ID(A3DModule::Input, 4);
		const static EventID WindowReleated = ID(A3DModule::Input, 5);

		struct KeyEventData
		{
			int key;
			int scancode;
			unsigned qualifier;
			bool repeat;
		};

		struct MouseEventData
		{
			unsigned cur_key;
			unsigned times;
			unsigned keys;
			unsigned qualifiter;
		};

		struct WindowReleatedData
		{
			unsigned evt_type;
		};
	};

}
