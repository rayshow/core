#pragma once

#include<ccdk/mpl/util/hash.h>
#include<ccdk/mpl/design_pattern/singleton.h>
#include<ccdk/mpl/function/function.h>
#include<ccdk/mpl/smart_ptr/poly_share_ptr.h>
#include<aurora3d/module.h>

a3d_namespace_evt_start

	using namespace mpl;

	using event_id_t       = uint32;
	using event_data_t     = sp::poly_share_ptr<void>;
	using event_callback_t = fn::function<void(event_data_t)>;
	
#define A3D_EVT_ID(mudule, sub)  ((((event_id_t)mudule)<<16)|((event_id_t)sub))

	//main table
	struct Module {
		enum {
			eCommon = 0,
			eWindows,
			eGraphics,
			eEngine,
			eInput,
		};
	};

	//sub-table of Common
	struct CommonEvt {
		enum {
			eFatalError = A3D_EVT_ID(Module::eCommon, 1),
			eQuit,
		};
	};


	//a callback item
	class event_callback_wrapper
	{
	private:
		void*            subscriber;
		event_callback_t callback;
	public:

		A3D_FORCEINLINE	event_callback_wrapper(
			void* subscriber, event_callback_t callback):
			subscriber(subscriber), callback(callback)
		{}
		
		void* get_subscriber() const { return subscriber;  }

		void operator()(event_data_t data) const {
			callback(data);
		}

		bool operator==(const event_callback_wrapper& r) const {
			return subscriber == r.subscriber;
		}

		ptr::size_t to_hash() noexcept {
			return util::hash(subscriber);
		}
	};

a3d_namespace_evt_end