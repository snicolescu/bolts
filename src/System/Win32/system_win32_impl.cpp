#include "System/Win32/system_win32_impl.h"
#include "System/system.h"

#include <algorithm>

//Define the graphics context, if any
#ifdef BOLTS_OPENGL

#include "System\Win32\system_win32_impl_opengl.h"

#endif

#include <iostream>

namespace Bolts {
	namespace System {

		const char *Win32SystemImpl::CLASS_NAME = "Bolts_Window";

		Win32SystemImpl::Win32SystemImpl() : m_bIsInitialized( false ), m_fullscreenWindow( NULL ), m_windowHandles()
			, m_windowEventListeners(), m_mouseEventListeners()
		{
			InitializeGraphicsContext();
		}

		Win32SystemImpl::~Win32SystemImpl()
		{
			if ( m_fullscreenWindow != NULL ) {
				ChangeDisplaySettings( NULL, 0 );
			}

			if ( m_bIsInitialized ) {
				graphicsContext.DestroyContext ();

				for ( auto it = m_windowHandles.begin(), end = m_windowHandles.end(); it != end; it++ ) {
					DestroyWindow( *it );
				}

				UnregisterClassA( Win32SystemImpl::CLASS_NAME, GetModuleHandle( NULL ) );
			}
		}

		void Win32SystemImpl::InitApp()
		{
			RegisterWindowClass();
			m_bIsInitialized = true;
		}

		windowHandle_t Win32SystemImpl::NewWindow( const std::string &name, uint32_t windowWidth, uint32_t widowHeight, uint8_t flags )
		{
			if ( !m_bIsInitialized ) {
				//TODO LOG: Cannot create a window ! Win32System not initialized
				return INVALID_WINDOW_HANDLE;
			}

			// Compute position and size
			HDC screenDC = GetDC( NULL );
			int left   = ( GetDeviceCaps( screenDC, HORZRES ) - windowWidth )  / 2;
			int top    = ( GetDeviceCaps( screenDC, VERTRES ) - widowHeight ) / 2;
			ReleaseDC( NULL, screenDC );

			// Choose the window style according to the Style parameter
			DWORD win32Style = WS_VISIBLE;
			if ( flags != Window::S_NONE ) {
				if ( flags & Window::S_TITLE ) {
					win32Style |= WS_CAPTION;
				}
				if ( flags & Window::S_RESIZEABLE ) {
					win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
				}
				if ( flags & Window::S_MINIMIZE ) {
					win32Style |= WS_MINIMIZEBOX;
				}
				if ( flags & Window::S_MAXIMIZE ) {
					win32Style |= WS_MAXIMIZEBOX;
				}
				if ( flags & Window::S_CLOSE ) {
					win32Style |= WS_SYSMENU;
				}
			}

			// In windowed mode, adjust width and height so that window will have the requested client area
			RECT rectangle = {0, 0, windowWidth, widowHeight};
			AdjustWindowRect( &rectangle, win32Style, false );
			windowWidth  = rectangle.right - rectangle.left;
			widowHeight = rectangle.bottom - rectangle.top;

			HWND m_handle = CreateWindowA( Win32SystemImpl::CLASS_NAME, name.c_str(), win32Style, left, top, windowWidth, widowHeight, NULL, NULL, GetModuleHandle( NULL ), this );
			m_windowHandles.insert( m_handle );

			return ( windowHandle_t ) m_handle;
		}

		void Win32SystemImpl::ShowCursor( windowHandle_t handle, bool bShow )
		{
			if ( !HandleIsValid( handle ) ) {
				//TODO LOG: Invalid window handle provided
			}

			if ( bShow ) {
				SetCursor( LoadCursor( NULL, IDC_ARROW ) );
			} else {
				SetCursor( NULL );
			}
		}

		void Win32SystemImpl::Update(  windowHandle_t handle )
		{
			if ( !HandleIsValid( handle ) ) {
				//TODO LOG: Invalid window handle provided
				return;
			}

			ProcessMessages( ( HWND ) handle );
			//glfwSwapBuffers();
		}

		bool Win32SystemImpl::HandleIsValid( windowHandle_t handle )
		{
			return ( handle != Win32SystemImpl::INVALID_WINDOW_HANDLE );
		}

		void Win32SystemImpl::ProcessMessages( HWND windowsHandle )
		{
			MSG message;
			while ( PeekMessage( &message, windowsHandle, 0, 0, PM_REMOVE ) ) {
				TranslateMessage( &message );
				DispatchMessage( &message );
			}
		}

		void Win32SystemImpl::RegisterWindowClass()
		{
			WNDCLASSA windowClass;
			windowClass.style         = 0;
			windowClass.lpfnWndProc   = &Win32SystemImpl::GlobalOnEventCallback;
			windowClass.cbClsExtra    = 0;
			windowClass.cbWndExtra    = 0;
			windowClass.hInstance     = GetModuleHandle( NULL );
			windowClass.hIcon         = NULL;
			windowClass.hCursor       = 0;
			windowClass.hbrBackground = 0;
			windowClass.lpszMenuName  = NULL;
			windowClass.lpszClassName = Win32SystemImpl::CLASS_NAME;
			RegisterClassA( &windowClass );
		}

		LRESULT CALLBACK Win32SystemImpl::GlobalOnEventCallback( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
		{
			// Associate handle and Window instance when the creation message is received
			if ( message == WM_CREATE ) {
				// Get Win32SystemImpl instance (it was passed as the last argument of CreateWindow)
				LONG_PTR window = ( LONG_PTR )reinterpret_cast<CREATESTRUCT *>( lParam )->lpCreateParams;

				// Set as the "user data" parameter of the window
				SetWindowLongPtr( handle, GWLP_USERDATA, window );
			}

			// Get the Win32SystemImpl instance corresponding to the window handle
			Win32SystemImpl *windowSystem = reinterpret_cast<Win32SystemImpl *>( GetWindowLongPtr( handle, GWLP_USERDATA ) );


			if ( !HandleIsValid ( ( windowHandle_t ) handle ) ) {
				//TODO LOG: System asked to handle invalid handle
			} else {
				// Forward the event to the appropriate function
				if ( windowSystem ) {
					windowSystem->HandleSystemEvent( handle, message, wParam, lParam );
				}
			}

			// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
			if ( message == WM_CLOSE ) {
				return 0;
			}

			//static const bool hasUnicode = hasUnicodeSupport();
			return DefWindowProcA( handle, message, wParam, lParam );
		}

		void Win32SystemImpl::HandleSystemEvent( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
		{
			switch ( message ) {
					// Set cursor event
				case WM_SETCURSOR : {
					// The mouse has moved, if the cursor is in our window we must refresh the cursor
					//if (LOWORD(lParam) == HTCLIENT)
					//SetCursor(m_cursor);
					break;
				}

				// Close event
				case WM_CLOSE : {
					Input::windowEvent_t event;
					event.type = Input::WE_CLOSED;
					event.windowHandle = ( windowHandle_t ) handle;
					PushWindowEvent( event );
					break;
				}

				// When user stops resizing
				case WM_EXITSIZEMOVE: {
					dimension2d_t newSize = GetWindowSize ( ( windowHandle_t ) handle );
					Input::windowEvent_t event;
					event.type		= Input::WE_RESIZED;
					event.value1	= newSize.width;
					event.value2	= newSize.height;
					event.windowHandle = ( windowHandle_t ) handle;
					PushWindowEvent( event );

					break;
				}

				// Gain focus event
				case WM_SETFOCUS : {
					Input::windowEvent_t event;
					event.type = Input::WE_GOTFOCUS;
					event.windowHandle = ( windowHandle_t ) handle;
					PushWindowEvent( event );

					break;
				}

				// Lost focus event
				case WM_KILLFOCUS : {
					Input::windowEvent_t event;
					event.type = Input::WE_LOSTFOCUS;
					event.windowHandle = ( windowHandle_t ) handle;
					PushWindowEvent( event );

					break;
				}


				// Mouse wheel event
				case WM_MOUSEWHEEL : {
					Input::mouseEvent_t event;
					event.state = Input::MS_SCROLLED;
					event.button = Input::B_LEFT;
					event.mouseX = 0;
					event.mouseY = ( ( int16_t ) HIWORD( wParam ) ) / WHEEL_DELTA;
					PushMouseEvent( event );
					break;
				}

				// Mouse wheel event
				case WM_MOUSEHWHEEL : {
					Input::mouseEvent_t event;
					event.state = Input::MS_SCROLLED;
					event.button = Input::B_LEFT;
					event.mouseX = ( ( int16_t ) HIWORD( wParam ) ) / WHEEL_DELTA;
					event.mouseY = 0;
					PushMouseEvent( event );
					break;
				}

				// Mouse left button down event
				case WM_LBUTTONDOWN : {
					Input::mouseEvent_t event;
					event.state = Input::MS_PRESSED;
					event.button = Input::B_LEFT;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
					break;
				}

				// Mouse left button up event
				case WM_LBUTTONUP : {
					Input::mouseEvent_t event;
					event.state = Input::MS_RELEASED;
					event.button = Input::B_LEFT;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
					break;
				}

				// Mouse right button down event
				case WM_RBUTTONDOWN : {
					Input::mouseEvent_t event;
					event.state = Input::MS_PRESSED;
					event.button = Input::B_RIGHT;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
					break;
				}

				// Mouse right button up event
				case WM_RBUTTONUP : {
					Input::mouseEvent_t event;
					event.state = Input::MS_RELEASED;
					event.button = Input::B_RIGHT;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
					break;
				}

				// Mouse wheel button down event
				case WM_MBUTTONDOWN : {
					Input::mouseEvent_t event;
					event.state = Input::MS_PRESSED;
					event.button = Input::B_MIDDLE;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
					break;
				}

				// Mouse wheel button up event
				case WM_MBUTTONUP : {
					Input::mouseEvent_t event;
					event.state = Input::MS_RELEASED;
					event.button = Input::B_MIDDLE;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
					break;
				}

				// Mouse X button down event
				case WM_XBUTTONDOWN : {
					Input::mouseEvent_t event;
					event.state = Input::MS_PRESSED;
					event.button = HIWORD( wParam ) == XBUTTON1 ? Input::B_4 : Input::B_5;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
					break;
				}

				// Mouse X button up event
				case WM_XBUTTONUP : {
					Input::mouseEvent_t event;
					event.state = Input::MS_RELEASED;
					event.button = HIWORD( wParam ) == XBUTTON1 ? Input::B_4 : Input::B_5;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
					break;
				}

				// Mouse move event
				case WM_MOUSEMOVE : {
					Input::mouseEvent_t event;
					event.state = Input::MS_MOVED;
					event.button = Input::B_LEFT;
					event.mouseX = static_cast<int16_t>( LOWORD( lParam ) );
					event.mouseY = static_cast<int16_t>( HIWORD( lParam ) );
					PushMouseEvent( event );
				}
			}
		}

		void Win32SystemImpl::RegisterWindowEventListener( winEvtLnrPtr_t listener )
		{
			if ( listener == NULL ) {
				//TODO LOG: Null window listener pointer passed
				return;
			}
			m_windowEventListeners.insert( listener );
		}

		void Win32SystemImpl::UnregisterWindowEventListener( winEvtLnrPtr_t listener )
		{
			if ( listener == NULL ) {
				//TODO LOG: Null window listener pointer passed
				return;
			}
			m_windowEventListeners.erase( listener );
		}

		void Win32SystemImpl::PushWindowEvent( Input::windowEvent_t event )
		{
			//Event handle guaranteed to be OK
			std::for_each( m_windowEventListeners.begin (), m_windowEventListeners.end(), [event] ( winEvtLnrPtr_t listener ) {
				listener->HandleWindowEvent( event );
			} );
		}

		void Win32SystemImpl::RegisterMouseEventListener( mouseEvtLnrPtr_t listener )
		{
			if ( listener == NULL ) {
				//TODO LOG: Null window listener pointer passed
				return;
			}
			m_mouseEventListeners.insert( listener );
		}

		void Win32SystemImpl::UnregisterMouseEventListener( mouseEvtLnrPtr_t listener )
		{
			if ( listener == NULL ) {
				//TODO LOG: Null window listener pointer passed
				return;
			}
			m_mouseEventListeners.erase( listener );
		}

		void Win32SystemImpl::PushMouseEvent( Input::mouseEvent_t event )
		{
			std::for_each( m_mouseEventListeners.begin (), m_mouseEventListeners.end(), [event] ( mouseEvtLnrPtr_t listener ) {
				listener->OnMouseEvent( event );
			} );
		}

		void Win32SystemImpl::CloseWindow( windowHandle_t handle )
		{
			if( !HandleIsValid ( handle ) ) {
				//TODO LOG: Invalid handle passed to CloseWindow
				return;
			}

			DestroyWindow( ( HWND ) handle );
		}

		Bolts::System::dimension2d_t Win32SystemImpl::GetWindowSize( windowHandle_t handle ) const
		{
			RECT rect;
			GetClientRect( ( HWND ) handle, &rect );

			return dimension2d_t( ( uint16_t ) ( rect.right - rect.left ), ( uint16_t ) ( rect.bottom - rect.top ) );
		}

		std::vector<dimension2d_t> Win32SystemImpl::GetSupportedDisplayModes() const
		{
			DEVMODE result;
			std::vector<dimension2d_t> resolutions;

			EnumDisplaySettings( NULL, 0, &result );
			resolutions.push_back ( dimension2d_t( ( uint16_t ) result.dmPelsWidth, ( uint16_t ) result.dmPelsHeight ) );
			int i = 1;
			while ( EnumDisplaySettings( NULL, i, &result ) ) {
				if ( result.dmBitsPerPel == 32 &&
				     ( result.dmPelsWidth != resolutions.back ().width &&
				       result.dmPelsHeight != resolutions.back ().height ) ) {
					resolutions.push_back ( dimension2d_t( ( uint16_t ) result.dmPelsWidth, ( uint16_t ) result.dmPelsHeight ) );
				}
				i++;
			}
			return resolutions;
		}

		bool Win32SystemImpl::EnableFullscreen( windowHandle_t handle, dimension2d_t resolution ) //, bool vsync /*= false*/ )
		{
			if ( m_fullscreenWindow != NULL ) {
				//TODO LOG: Failed to change display mode to fullscreen.
				//			There is a window set to fullscreen already << Console::ENDL
				return false;
			}
			HWND wHandle = ( HWND ) handle;
			DEVMODE devMode;
			devMode.dmSize       = sizeof( devMode );
			devMode.dmPelsWidth  = resolution.width;
			devMode.dmPelsHeight = resolution.height;
			devMode.dmBitsPerPel = 32;
			devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			// Apply fullscreen mode
			if ( ChangeDisplaySettings( &devMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
				//TODO LOG: Failed to change display mode to fullscreen.
				//			Resolution: << resolution.width << " | " resolution.height << Console::ENDL
				return false;
			}

			// Remember the current fullscreen window
			//		And it's style, for restoring later
			m_fullscreenWindow = wHandle;
			WINDOWINFO winfo;
			winfo.cbSize = sizeof( WINDOWINFO );
			GetWindowInfo( wHandle, &winfo );
			m_fullscreenWindowOldStyle = winfo.dwStyle;
			m_fullscreenWindowOldSize = GetWindowSize ( handle );

			// Make the window flags compatible with fullscreen mode
			SetWindowLong( wHandle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
			SetWindowLong( wHandle, GWL_EXSTYLE, WS_EX_APPWINDOW );

			// Resize the window so that it fits the entire screen
			//		And update the changes to the style we made
			SetWindowPos( wHandle, HWND_TOP, 0, 0, resolution.width, resolution.height, SWP_FRAMECHANGED );
			ShowWindow( wHandle, SW_SHOW );

			return true;
		}

		void Win32SystemImpl::DisableFullscreen()
		{
			if ( m_fullscreenWindow == NULL ) {
				return;
			}

			ChangeDisplaySettings( NULL, 0 );
			//Restore old window style
			SetWindowLong( m_fullscreenWindow, GWL_STYLE, m_fullscreenWindowOldStyle );
			//Calling this function updates the changes we made previously
			//		Note that the parameters that we passed are ignored, thanks to the flags we have set
			UINT flags = SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER;
			SetWindowPos( m_fullscreenWindow, 0, 0, 0, m_fullscreenWindowOldSize.width, m_fullscreenWindowOldSize.height, flags );

			m_fullscreenWindow = NULL;
		}

		void Win32SystemImpl::InitializeGraphicsContext()
		{
			graphicsContext.Initialize();
		}

		bool Win32SystemImpl::CreateGraphicsContext( windowHandle_t handle )
		{
			assert( m_bIsInitialized );

			return graphicsContext.CreateContext ( ( HWND ) handle );
		}

		void Win32SystemImpl::DestroyGraphicsContext()
		{
			assert( m_bIsInitialized );

			graphicsContext.DestroyContext ();
		}

		bool Win32SystemImpl::SwapWindowBuffers()
		{
			return graphicsContext.SwapWindowBuffers ();
		}

	};
};