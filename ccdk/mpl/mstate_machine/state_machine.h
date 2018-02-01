#pragma once

#include<ccdk/mpl/mpl_module.h>

ccdk_namespace_mpl_msm_start

/* implements from mpl book */
template<typename T>
struct state_machine
{
	typedef state_machine this_type;
	typedef T             derive_type;

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

	template<typename Row, typename NextDispatcher>
	struct event_dispatcher
	{
		typedef typename Row::fsm_type   fsm_type;
		typedef typename Row::event_type event_type;

		constexpr void dispatch(fsm_type& fsm, int state, event_type const& evt) noexcept
		{
			if (state == Row::curr_state)
			{
				return Row::execute(fsm, evt);
			}
			return NextDispatcher::dispatch(fsm, state, evt);
		}
	};

	struct default_event_dispatcher
	{
		template<typename Event>
		constexpr void dispatch(this_type& fsm, int state, Event const& evt)
		{

		}
	};

	template<typename Table, typename Event>
	struct generate_dispather
	{};


	template<typename Event>
	constexpr void default_action(this_type& fsm, int state, Event const& evt)
	{
		static_cast<derive_type>(fsm).
	}

	template<typename Event>
	constexpr void process_event(Event const& evt)
	{

	}
};

ccdk_namespace_mpl_msm_end