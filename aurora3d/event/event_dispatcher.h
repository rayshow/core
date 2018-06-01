#pragma once

#include<ccdk/mpl/design_pattern/singleton.h>
#include<ccdk/mpl/iterator/algorithm/foreach.h>
#include<ccdk/container/unordered_map.h>
#include<ccdk/container/unordered_set.h>
#include<aurora3d/event/event_def.h>

a3d_namespace_evt_start

using namespace ccdk::ct;

//event dispatcher
class event_dispatcher :public dp::singleton<event_dispatcher>
{
	using subscripber_map = unordered_map<event_id_t, unordered_set< event_callback_wrapper > >;
private:
	subscripber_map subscribers{};
public:
	//subscribe to event with event id
	A3D_FORCEINLINE void subscribe_evt(
		event_id_t id, event_callback_wrapper& callback) {
		subscribers[id].emplace(callback);
	}

	//unsubscribe to event with event id
	A3D_FORCEINLINE void cancel_evt(event_id_t id, event_callback_wrapper callback) {
		auto item = subscribers.find(id);
		if (item == subscribers.end()) return;
		item->second().erase(callback);
	}

	//spread this event
	A3D_FORCEINLINE void trigger_evt(event_id_t id, event_data_t data) {
		auto item = subscribers.find(id);
		if (item == subscribers.end()) return;
		it::foreach(item->second(), [&](event_callback_wrapper const& callback) {
			callback(data);
		});
	}

	//notify to exit
	A3D_FORCEINLINE void notify_fatal_evt()
	{

		std::shared_ptr<Status> err{ new Status{} };
		err->code = Status::FatalError;
		err->msg = "fatal error happened!";
		TriggerEvent(CommonEvt::FatalError, err);
	}
};

typedef std::shared_ptr<EventDispatcher> EventDispatcherPtr;

#define CCDK_SUBSCRIBE_EVENT(evt_id, callback)  \
	EventDispatcher::GetSingletonPtr()->SubscribeEvent( \
		(evt_id), \
		EventCallBackWarpper{ (this), (std::bind( (callback), this, std::placeholders::_1)) }  \
	);
#define CCDK_CANCAL_EVENT(evt_id, callback) \
	EventDispatcher::GetSingletonPtr()->CancelEvent((evt_id), (callback))
#define CCDK_SEND_EVENT(evt_id, data) \
	EventDispatcher::GetSingletonPtr()->TriggerEvent((evt_id), (data))
#define CCDK_SEND_FATAL() \
	EventDispatcher::GetSingletonPtr()->NotifyFatalError()

a3d_namespace_evt_end