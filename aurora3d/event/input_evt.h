#pragma once

#include<aurora3d/event/event_def.h>
#include<aurora3d/module.h>
a3d_namespace_evt_start

struct InputEvt
{
	enum {
		eKeyPressed = A3D_EVT_ID(Module::eInput, 1),
		eKeyReleased,
		eMousePressed,
		eMouseReleased,
		eWindowReleated
	};

	struct key_evt_data {
		int32  key;
		int32  scancode;
		uint32 qualifier;
		bool   repeat;
	};

	struct mouse_evt_data {
		uint32 cur_key;
		uint32 times;
		uint32 keys;
		uint32 qualifiter;
	};

	struct window_releated_data {
		uint32 evt_type;
	};
};

a3d_namespace_evt_end
