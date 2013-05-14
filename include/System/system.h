#ifndef system_h__
#define system_h__

#include "Core/types.h"

namespace Bolts {
	namespace System {
		typedef uint64_t windowHandle_t;

		struct dimension2d_t {
			dimension2d_t(): width( 0 ), height( 0 ) {};
			dimension2d_t( uint16_t w, uint16_t h ): width( w ), height( h ) {};

			uint16_t width;
			uint16_t height;
		};
		struct position2d_t {
			int16_t x;
			int16_t y;
		};

		namespace Window {
			enum styleFlags_t {
			    S_NONE		= 0,
			    S_RESIZEABLE = 1,
			    S_TITLE		= 2,
			    S_CLOSE		= 1 << 2,
			    S_MAXIMIZE	= 1 << 3,
			    S_MINIMIZE	= 1 << 4
			};
		};
	};

	namespace Input {
		enum eventType_t { WE_RESIZED = 0, WE_CLOSED, WE_MINIMIZED, WE_LOSTFOCUS, WE_GOTFOCUS};

		//NOTE: If the event type is WE_RESIZED , value1 and value2 contain the width and the height, respectively
		struct windowEvent_t {
			System::windowHandle_t	windowHandle;
			int32_t					value1;
			int32_t					value2;
			eventType_t				type;
		};

		class WindowEventListener {
			public:
				virtual void	HandleWindowEvent( windowEvent_t ) = 0;
		};

	};
};

#endif // system_h__