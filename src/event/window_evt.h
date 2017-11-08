#pragma once
#include"event_def.h"
#include"../common_def.h"
#include"event_dispatcher.h"

namespace Aurora3D
{
	struct WindowEvt
	{
		const static EventID WindowCreated = ID(A3DModule::Window, 1);
		const static EventID DisplayChanged = ID(A3DModule::Window, 2);

		struct DisplayData
		{
			unsigned width;
			unsigned height;
			void* window_handle = nullptr;
		};
	};

}
