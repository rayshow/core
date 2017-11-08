#pragma once

#include<memory>
#include<functional>
#include<unordered_map>
#include<unordered_set>
#include"../design/singleton.h"
#include"event_def.h"

namespace Aurora3D
{
	///event dispatcher
	class EventDispatcher :public Singleton<EventDispatcher>
	{
		using EventSubScriberMap = std::unordered_map<EventID,
			std::unordered_set< EventCallBackWarpper > >;
	private:
		EventSubScriberMap subscribers_{};
	public:
		//subscribe to event with event id
		void SubscribeEvent(EventID evt_id, EventCallBackWarpper& callback)
		{
			subscribers_[evt_id].emplace(callback);
		}

		//unsubscribe to event with event id
		void CancalEvent(EventID evt_id, EventCallBackWarpper callback)
		{
			auto pos = subscribers_.find(evt_id);
			if (subscribers_.end() == pos) return;
			pos->second.erase(callback);
		}

		//spread this event
		void TriggerEvent(EventID evt_id, EventData data)
		{
			auto pos = subscribers_.find(evt_id);
			if (subscribers_.end() == pos) return;
			for (auto callback : pos->second)
			{
				callback(data);
			}
		}

		//notify to exit
		void NotifyFatalError()
		{
			std::shared_ptr<Status> err{ new Status{} };
			err->code = Status::FatalError;
			err->msg = "fatal error happened!";
			TriggerEvent(CommonEvt::FatalError, err);
		}
	};

	typedef std::shared_ptr<EventDispatcher> EventDispatcherPtr;

#define A3D_SUBSCRIBE_EVENT(evt_id, callback)  \
	EventDispatcher::GetSingletonPtr()->SubscribeEvent( \
		(evt_id), \
		EventCallBackWarpper{ (this), (std::bind( (callback), this, std::placeholders::_1)) }  \
	);
#define A3D_CANCAL_EVENT(evt_id, callback) \
	EventDispatcher::GetSingletonPtr()->CancelEvent((evt_id), (callback))
#define A3D_SEND_EVENT(evt_id, data) \
	EventDispatcher::GetSingletonPtr()->TriggerEvent((evt_id), (data))
#define A3D_SEND_FATAL() \
	EventDispatcher::GetSingletonPtr()->NotifyFatalError()
}