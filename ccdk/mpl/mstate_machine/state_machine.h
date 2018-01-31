#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_msm_start

template<typename T>
struct state_machine
{
	template< int CurrentState, class Event, int NextState, void(T::*Action)(Event const&)>
	struct row
	{
		static constexpr int curr_state = CurrentState;
		static constexpr int next_state = NextState;
		typedef Event event_type;
		typedef T     fsm_type;

		CCDK_FORCEINLINE constexpr void execute(fsm_type& fsm, Event const& evt) const
		{
			(fsm.*Action)(evt);
		}
	};
};

ccdk_namespace_mpl_msm_end