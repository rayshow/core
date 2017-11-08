#pragma once

#include<memory>
#include<functional>
#include<unordered_map>
#include<unordered_set>
#include"../design/singleton.h"
#include"../common_def.h"


namespace Aurora3D
{
	typedef size_t EventID;
	typedef std::shared_ptr<void> EventData;
	typedef std::function<void(EventData)> EventCallBack;
	
#define ID(mudule, sub)  ((mudule<<8)| (sub))

	struct A3DModule
	{
		const static unsigned Common = 0;
		const static unsigned Window = 1;
		const static unsigned Graphics = 2;
		const static unsigned Engine = 3;
		const static unsigned Input = 4;
	};

	///fatal error, immedicately notify and stop
	struct CommonEvt
	{
		const static EventID FatalError = ID(A3DModule::Common, 1);
		const static EventID Quit = ID(A3DModule::Common, 2);
	};


	///a callback item
	class EventCallBackWarpper
	{
	private:
		void* subscriber_;
		EventCallBack callback_;
	public:
		EventCallBackWarpper(void* subscriber, EventCallBack callback) :
			subscriber_(subscriber), callback_(callback)
		{}
		
		void* GetSubcriber() const { return subscriber_;  }

		void operator()(EventData data) const
		{
			callback_(data);
		}

		bool operator==(const EventCallBackWarpper& r) const
		{
			return subscriber_ == r.subscriber_;
		}

	};
}

namespace std
{
	///for unorder_xxx key
	template<>
	struct hash<Aurora3D::EventCallBackWarpper>
	{
		size_t operator()(const Aurora3D::EventCallBackWarpper& warpper)const
		{
			return std::hash<void*>()(warpper.GetSubcriber());
		}
	};
}
