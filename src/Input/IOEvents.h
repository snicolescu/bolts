#ifndef events_h__
#define events_h__

#include "Core/types.h"

namespace Bolts {
	namespace Input {
		enum mouseButton_t { B_LEFT = 0, B_RIGHT, B_MIDDLE, B_4, B_5};
		enum mouseState_t { MS_PRESSED = 0, MS_RELEASED, MS_MOVED, MS_SCROLLED, MS_CONNECTED, MS_DISCONNECTED };

		/*
		 *	NOTE: When the button state is > MS_MOVED, ignore the value of mouseButton
		 *	NOTE: When the button is MB_SCROLL , mouseX and Y contain the deltas for their respective axes
		 */
		struct mouseEvent_t {
			mouseState_t	state;
			mouseButton_t	button;
			int16_t			mouseX;
			int16_t			mouseY;
			uint8_t			mouseId;
		};

		class MouseEventListener {
			public:
				virtual void	OnMouseEvent( mouseEvent_t ) = 0;
		};
	};
};

#endif // events_h__
