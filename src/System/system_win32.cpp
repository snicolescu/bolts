//#include "stdafx.h"
#include "System/system_win32.h"
#include "System/Win32/system_win32_impl.h"

/*
#include <glew/GL/glew.h>
#include <windows.h>
#include <GLFW/glfw.h>
*/
#include <iostream>

namespace Bolts {
	namespace System {

		Win32System::Win32System() : m_impl( new Win32SystemImpl() )
		{

		}

		Win32System::~Win32System()
		{

		}

		void Win32System::InitApp()
		{
			m_impl->InitApp ();
		}

		windowHandle_t Win32System::NewWindow( const std::string &name, uint32_t screenWidth, uint32_t screenHeight, uint8_t flags/* = S_CLOSE */ )
		{
			return m_impl->NewWindow ( name, screenWidth, screenHeight, flags );
		}

		void Win32System::HideCursor( windowHandle_t handle )
		{
			m_impl->ShowCursor( handle, false );
		}

		void Win32System::ShowCursor( windowHandle_t handle )
		{
			m_impl->ShowCursor( handle, true );
		}

		void Win32System::Update( windowHandle_t handle )
		{
			m_impl->Update ( handle );
		}

		void Win32System::RegisterWindowEventListener( Input::WindowEventListener *listener )
		{
			m_impl->RegisterWindowEventListener( listener );
		}

		void Win32System::UnregisterWindowEventListener( Input::WindowEventListener *listener )
		{
			m_impl->UnregisterWindowEventListener( listener );
		}

		void Win32System::CloseWindow( windowHandle_t handle )
		{
			m_impl->CloseWindow( handle );
		}

		void Win32System::RegisterMouseEventListener( Input::MouseEventListener *listener )
		{
			m_impl->RegisterMouseEventListener( listener );
		}

		void Win32System::UnregisterMouseEventListener( Input::MouseEventListener *listener )
		{
			m_impl->UnregisterMouseEventListener( listener );
		}

		Bolts::System::dimension2d_t Win32System::GetWindowSize( windowHandle_t handle ) const
		{
			return m_impl->GetWindowSize( handle );
		}

		std::vector<dimension2d_t> Win32System::GetSupportedDisplayModes() const
		{
			return m_impl->GetSupportedDisplayModes();
		}

		bool Win32System::EnableFullscreen( windowHandle_t handle, dimension2d_t resolution)//, bool vsync /*= false*/ )
		{

			return m_impl->EnableFullscreen( handle, resolution);//, vsync );
		}

		void Win32System::DisableFullscreen()
		{
			m_impl->DisableFullscreen();
		}

		bool Win32System::CreateGraphicsContext( windowHandle_t handle)
		{
			return m_impl->CreateGraphicsContext (handle);
		}

		void Win32System::DestroyGraphicsContext()
		{
			m_impl->DestroyGraphicsContext ();
		}

		bool Win32System::SwapWindowBuffers()
		{
			return m_impl->SwapWindowBuffers ();
		}

	};
};
