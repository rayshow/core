#pragma once

#include<aurora3d/event/event_def.h>
#include<aurora3d/module.h>

a3d_namespace_evt_start

struct EngineEvt
{
	enum {
		eFrameBegin = A3D_EVT_ID(Module::eEngine, 1),
		eFrameEnd,
	};

	struct frame_begin_data {
		float    delta_time;
		float    total_time;
		unsigned total_frame;
	};
};

a3d_namespace_evt_end
