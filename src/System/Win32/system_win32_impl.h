#ifndef system_win32_impl_h__
#define system_win32_impl_h__

#include "Core/types.h"
#include "System/system.h"
#include "Input/IOEvents.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>
#include <set>
#include <map>

namespace Bolts {
	namespace System {

		//TODO: Look into sharing a graphics context and rendering on other windows
		//TODO: Add Depth Bits and Stencil bits option
		//TODO: Add MSAA, Debug Output and sRGB Framebuffer options
		class Win32SystemImpl {
			public:
				typedef Input::WindowEventListener	*winEvtLnrPtr_t;
				typedef Input::MouseEventListener	*mouseEvtLnrPtr_t;

				static const windowHandle_t			INVALID_WINDOW_HANDLE = NULL;
				static const char					*CLASS_NAME;

				Win32SystemImpl();
				~Win32SystemImpl();

				void						InitApp();
				void						Update( windowHandle_t );

				windowHandle_t				NewWindow( const std::string &, uint32_t sw, uint32_t sh, uint8_t flags );
				void						CloseWindow( windowHandle_t handle );
				dimension2d_t				GetWindowSize( windowHandle_t ) const;
				void						ShowCursor( windowHandle_t, bool );

				std::vector<dimension2d_t>	GetSupportedDisplayModes() const;
				bool						EnableFullscreen( windowHandle_t, dimension2d_t resolution ); //, bool vsync = false );
				void						DisableFullscreen();

				//******* Graphics *****
				//Return true if context initialized successfully
				bool						CreateGraphicsContext( windowHandle_t );
				void						DestroyGraphicsContext();

				bool						SwapWindowBuffers();

				//Event-related functions
				void						RegisterWindowEventListener( winEvtLnrPtr_t );
				void						UnregisterWindowEventListener( winEvtLnrPtr_t );
				void						RegisterMouseEventListener( mouseEvtLnrPtr_t );
				void						UnregisterMouseEventListener( mouseEvtLnrPtr_t );

			private:
				typedef std::set<winEvtLnrPtr_t>	windowEventListeners_t;
				typedef std::set<mouseEvtLnrPtr_t>	mouseEventListeners_t;

				//Window related helpers
				static LRESULT CALLBACK		GlobalOnEventCallback( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
				static bool					HandleIsValid( windowHandle_t );

				//Graphics
				void						InitializeGraphicsContext();

				void						RegisterWindowClass();
				void						ProcessMessages( HWND );
				void						HandleSystemEvent( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
				void						PushWindowEvent( Input::windowEvent_t );
				void						PushMouseEvent( Input::mouseEvent_t );

				bool					m_bIsInitialized;
				std::set< HWND >		m_windowHandles;
				HWND					m_fullscreenWindow;
				DWORD 					m_fullscreenWindowOldStyle;
				dimension2d_t			m_fullscreenWindowOldSize;
				windowEventListeners_t	m_windowEventListeners;
				mouseEventListeners_t	m_mouseEventListeners;
		};
	}
};
#endif // system_win32_impl_h__