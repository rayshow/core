#include<cstdio>
#include<ccdk/debug_type.hpp>
#include<ccdk/mpl/mstate_machine/state_machine.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::msm;

struct play_event {};
struct open_close_event {};
struct cd_detected_event {};
struct pause_event {};
struct stop_event {};

class player : public state_machine< player>
{
private:
	friend struct state_machine<player>;

	void start_play(play_event const&) {}
	void open_drawer(open_close_event const&) {}
	void close_drawer(open_close_event const&) {}
	void store_cd_info(cd_detected_event const&) {}
	void stop_palyback(stop_event const&) {}
	void pause_playback(pause_event const&) {}
	void resume_playback(play_event const&) {}
	void stop_and_open(open_close_event const&) {}
	template<typename Event>
	void default_event(Event const& evt) {}

	enum { empty, open, stopped, playing, pause, initial_state };

	typedef arg_pack<
		//     source   trigger     dest        
		row< stopped, play_event, playing, &player::start_play>,
		row< stopped, open_close_event, playing, &player::open_drawer>,
		row< open, open_close_event, empty, &player::close_drawer>,
		row< empty, open_close_event, open, &player::open_drawer>,
		row< empty, cd_detected_event, stopped, &player::store_cd_info>,
		row< playing, stop_event, stopped, &player::stop_palyback>,
		row< playing, pause_event, pause, &player::pause_playback>,
		row< playing, open_close_event, open, &player::stop_and_open>,
		row< pause, play_event, playing, &player::resume_playback>,
		row< pause, stop_event, stopped, &player::stop_palyback>,
		row< pause, open_close_event, open, &player::stop_and_open>
	> transition_table;
public:
	player() :state_machine(stopped) {}
};

template<typename T>
struct test_mfn { static constexpr bool value = true; };

template<CCDK_TFN(TFn), typename ... Args>
struct test_mfn<TFn<Args...>> {
	static constexpr bool value = false;
};

int main()
{
	player p{};
	p.process_event(open_close_event{});
	p.process_event(play_event{});
	int v = 0;
	DebugValue(v = test_mfn< add_apply_<is_placeholder_>>::value);

	getchar();
	return 0;
}