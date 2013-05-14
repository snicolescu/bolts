#ifndef system_win32_h__
#define system_win32_h__

#include "Core/types.h"
#include "System\system.h"
#include "Input\events.h"

//#include <loki\Pimpl.h>
#include <string>
#include <vector>

namespace Bolts {
	namespace System {

		class Win32SystemImpl;

		//TODO: Event Management
		//TODO: Hiding cursor per window
		class Win32System {
			public:
				static const windowHandle_t INVALID_WINDOW_HANDLE = 0;

				Win32System();
				~Win32System();

				void						InitApp();
				void						Update( windowHandle_t );

				windowHandle_t				NewWindow( const std::string &, uint32_t sw = 800, uint32_t sh = 600, uint8_t flags = Window::S_CLOSE );
				void						CloseWindow( windowHandle_t );
				dimension2d_t				GetWindowSize( windowHandle_t ) const;
				//NOTE: Right now, it shows/hides the cursor for all the windows
				void						HideCursor( windowHandle_t );
				void						ShowCursor( windowHandle_t );

				std::vector<dimension2d_t>	GetSupportedDisplayModes() const;
				//NOTE: VSync isn't currently supported, Bit Depth is set to 32
				bool						EnableFullscreen( windowHandle_t, dimension2d_t resolution);//, bool vsync = false );
				void						DisableFullscreen();

				//******* Graphics *****
				//Return true if context initialized successfully
				bool						CreateGraphicsContext( windowHandle_t);
				void						DestroyGraphicsContext();

				//Returns false on  failure. Fail reason details not implemented yet.
				bool						SwapWindowBuffers();

				void						RegisterWindowEventListener( Input::WindowEventListener * );
				void						UnregisterWindowEventListener( Input::WindowEventListener * );
				void						RegisterMouseEventListener( Input::MouseEventListener * );
				void						UnregisterMouseEventListener( Input::MouseEventListener * );

			private:
				std::auto_ptr<Win32SystemImpl>	m_impl;
		};

	};
};

#endif // system_win32_h__

