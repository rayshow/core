#pragma once

#include<ccdk/debug_type.hpp>
#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/apply_.h>
#include<ccdk/mpl/mcontainer/algorithm/fold_.h>
#include<ccdk/mpl/mcontainer/algorithm/lambda_.h>
#include<ccdk/mpl/mcontainer/view/filter_view_.h>
#include<ccdk/mpl/mcontainer/iterator_.h>

ccdk_namespace_mpl_msm_start

/* implements from mpl book */
template<typename T>
struct state_machine
{
protected:
	typedef state_machine this_type;
	typedef T             derive_type;

	int state;

	state_machine() :state{ derive_type::initial_state } {}


	template< int CurrentState, class Event, int NextState, void(T::*Action)(Event const&)>
	struct row
	{
		static constexpr int curr_state = CurrentState;
		static constexpr int next_state = NextState;
		typedef Event event_type;

		CCDK_FORCEINLINE static void execute(derive_type& fsm, Event const& evt)
		{
			(fsm.*Action)(evt);
		}
	};

	template<typename Row, typename NextDispatcher>
	struct event_dispatcher
	{
		typedef typename Row::event_type event_type;

		CCDK_FORCEINLINE static void dispatch(derive_type& fsm, int state, event_type const& evt)
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
		CCDK_FORCEINLINE static void dispatch(this_type& fsm, int state, Event const& evt)
		{
			fsm.default_action(fsm, state, evt);
		}
	};

	template<typename T2, typename Event>
	struct match_event :is_same< Event, typename T2::event_type> {};

	template<typename Table, typename Event>
	struct generate_dispather: 
				reverse_fold_right_< filter_view_<Table, match_event<__, Event> >,
							default_event_dispatcher,
							event_dispatcher<__,__> >
	{};


	template<typename Event>
	constexpr void default_action(this_type& fsm, int state, Event const& evt)
	{
		
	}

public:
	template<typename Event>
	constexpr void process_event(Event const& evt)
	{
		typedef typename generate_dispather<typename derive_type::transition_table, Event>::type dispatcher;
		dispatcher::dispatch(*static_cast<derive_type*>(this), state, evt);
	}
};

ccdk_namespace_mpl_msm_end