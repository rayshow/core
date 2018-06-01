#pragma once
#include<vector>
#include<chrono>
#include<thread>
#include<aurora3d/module.h>

a3d_namespace_engine_start

struct PulserFruquency
{
	static const unsigned Smooth = 40;
	static const unsigned Fast = 80;
	static const unsigned Unlimited = 1500;
};

///nano timer
class pulser
{
	using clock_type = std::chrono::high_resolution_clock;
	using duration = clock_type::duration;
	using timepoint = clock_type::time_point;
	using period = clock_type::period;
private:
	timepoint start_point{};
	timepoint finish_point{};
	duration  loop_duration{1};
	duration  elapse{};
	uint32 count = 0;
	uint32 fruquency = PulserFruquency::Smooth;

public:
	A3D_FORCEINLINE pulser() noexcept {
		start_point = clock_type::now();
		loop_duration *= period::den / (double)fruquency;
	}

	A3D_FORCEINLINE void tick() {
		elapse = clock_type::now() - start_point;
		start_point = clock_type::now();
	}

	A3D_FORCEINLINE void sleep() {
		finish_point = clock_type::now();
		duration execute_time = finish_point - start_point;
		std::chrono::duration<long long, std::milli> one_milli(1);
		if (execute_time < loop_duration)
		{
			std::this_thread::sleep_for(loop_duration-execute_time);
		}
	}

	// 1 / 1000000000
	CCDK_FORCEINLINE int64 elapsed_nano_seconds() {
		return elapse.count()*period::num;
	}

	// 1 / 1000000
	CCDK_FORCEINLINE double elapsed_micro_seconds() {
		std::chrono::duration<double, std::micro> seconds(elapse);
		return seconds.count();
	}

	// 1 / 1000
	CCDK_FORCEINLINE double elapsed_milli_seconds() {
		std::chrono::duration<double, std::milli> seconds(elapse);
		return seconds.count();
	}

	CCDK_FORCEINLINE double elapsed_seconds() {
		std::chrono::duration<double, std::ratio<1, 1>> seconds(elapse);
		return seconds.count();
	}

	CCDK_FORCEINLINE double instant_fps() {
		return period::den / ((double)elapse.count());
	}

	CCDK_FORCEINLINE double fruquency_elapse() {
		return (double)loop_duration.count() / period::den;
	}
};

a3d_namespace_engine_end