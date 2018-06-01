#pragma once

#include<aurora3d/event/event_def.h>
#include<aurora3d/module.h>
a3d_namespace_evt_start

struct WindowEvt
{
	enum {
		eCreated = A3D_EVT_ID(Module::eWindows, 1),
		eDisplayChanged,
	};

	struct display_data {
		uint32 width;
		uint32 height;
		void* window_handle = nullptr;
	};
};

a3d_namespace_evt_end
