#pragma once
#include <vector>
#include <cassert>
#include<chrono>
#include<thread>
#include"../common_def.h"

namespace Aurora3D
{
	struct PulserFruquency
	{
		static const unsigned Smooth = 40;
		static const unsigned Fast = 80;
		static const unsigned Unlimited = 1500;
	};

	///nano timer
	class Pulser
	{
		using Clock = std::chrono::high_resolution_clock;
	private:
		Clock::duration elapse{};
		unsigned count = 0;
		unsigned fruquency = PulserFruquency::Smooth;
		Clock::time_point start_point{};
		Clock::time_point finished_point{};
		Clock::duration loop_duration{1};
	public:
		Pulser() 
		{
			start_point = Clock::now();
			loop_duration *= Clock::period::den / (double)fruquency;
		}

		CCDK_FORCEINLINE void Pulse()
		{
			elapse = Clock::now() - start_point;
			start_point = Clock::now();
		}

		CCDK_FORCEINLINE void Sleep()
		{
			finished_point = Clock::now();
			Clock::duration execute_time = finished_point - start_point;
			std::chrono::duration<long long, std::milli> one_milli(1);
			if (execute_time < loop_duration)
			{
				std::this_thread::sleep_for(loop_duration-execute_time);
			}
		}

		CCDK_FORCEINLINE long long GetElapseNanoSeconds()
		{
			return elapse.count()*Clock::period::num;
		}

		CCDK_FORCEINLINE double GetElapseMicroSeconds()
		{
			std::chrono::duration<double, std::micro> seconds(elapse);
			return seconds.count();
		}

		CCDK_FORCEINLINE double GetElapseMilliSeconds()
		{
			std::chrono::duration<double, std::milli> seconds(elapse);
			return seconds.count();
		}

		CCDK_FORCEINLINE double GetElapseSeconds()
		{
			std::chrono::duration<double, std::ratio<1, 1>> seconds(elapse);
			return seconds.count();
		}

		CCDK_FORCEINLINE double GetFPS()
		{
			return Clock::period::den / ((double)elapse.count());
		}

		CCDK_FORCEINLINE double GetFruquencyElapse()
		{
			return (double)loop_duration.count() / Clock::period::den;
		}
	};

}