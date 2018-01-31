#include<stdio.h>
#include<ccdk/debug_type.hpp>
#include<ccdk/mpl/units/quantity.h>
#include<ccdk/mpl/mstate_machine/state_machine.h>
#include<ccdk/mpl/mcontainer/arg_pack.h>

using namespace ccdk;
using namespace ccdk::mpl;
using namespace ccdk::mpl::units;
using namespace ccdk::mpl::msm;

struct play_event {};
struct open_close_event {};
struct cd_detected_event {};
struct pause_event {};
struct stop_event {};

class player : public state_machine< player>
{
public:
	template<typename>
	friend struct state_machine;

private:

	void start_play(play_event const&) {}
	void open_drawer(open_close_event const&) {}
	void close_drawer(open_close_event const&) {}
	void store_cd_info(cd_detected_event const&) {}
	void stop_palyback(stop_event const&) {}
	void pause_playback(pause_event const&) {}
	void resume_playback(play_event const&) {}
	void stop_and_open(open_close_event const& ){}

	enum { empty, open, stopped, playing, pause };

	typedef arg_pack<
		row< stopped, play_event,        playing, &start_play>,
		row< stopped, open_close_event,  playing, &open_drawer>,
		row< open,    open_close_event,  empty,   &close_drawer>,
		row< empty,   open_close_event,  open,    &open_drawer>,
		row< empty,   cd_detected_event, stopped, &store_cd_info>,
		row< playing, stop_event,	     stopped, &stop_palyback>,
		row< playing, pause_event,       pause,   &pause_playback>,
		row< playing, open_close_event,  open,    &stop_and_open>,
		row< pause,	  play_event,        playing, &resume_playback>,
		row< pause,   stop_event,        stopped, &stop_palyback>,
		row< pause,   open_close_event,  open,    &stop_and_open>
	> transition_table;

};

int main()
{
	ulength len = 100_m;
	length<kilo> len2 = 1_km;
	DebugValue(len);
	DebugValue(len2);
	
	getchar();
	return 0;
}