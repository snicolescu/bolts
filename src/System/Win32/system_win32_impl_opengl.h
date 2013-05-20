#ifndef system_win32_impl_opengl_h__
#define system_win32_impl_opengl_h__

#include <glew/GL/glew.h>
#include <assert.h>

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

namespace Bolts {

	namespace {
		class ContextOpenGL {
			public:
				void	Initialize() {
					ResetHandles();
				}

				//NOTE: The thread which calls this function becomes the graphics context owning thread
				bool	CreateContext( HWND );
				void	DestroyContext();

				bool	SwapWindowBuffers ();
			private:
				void	ResetHandles();

				HWND	m_windowHandle;
				HDC		m_deviceContext;
				HGLRC 	m_renderContext;
		};

		static ContextOpenGL graphicsContext;

		bool ContextOpenGL::CreateContext( HWND whandle )
		{
			//TODO LOG:Trying to create a context after one has already been created
			assert( m_windowHandle == 0 );
			//TODO LOG:Trying to create a context for a blank window handle
			assert( whandle != 0 );

			// get the device context (DC)
			m_deviceContext = GetDC( whandle );
			if ( !m_deviceContext ) {
				//TODO LOG: Error getting device context
				return false;
			}
			// remember the window handle (HWND)
			m_windowHandle = whandle;

			// set the pixel format for the DC
			PIXELFORMATDESCRIPTOR pfd;
			ZeroMemory( &pfd, sizeof( pfd ) );
			pfd.nSize = sizeof( pfd );
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
			              PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 24;
			pfd.cDepthBits = 16;
			pfd.iLayerType = PFD_MAIN_PLANE;
			int format = ChoosePixelFormat( m_deviceContext, &pfd );
			SetPixelFormat( m_deviceContext, format, &pfd );

			// create the render context (RC)
			m_renderContext = wglCreateContext( m_deviceContext );
			if ( !m_renderContext ) {
				//TODO LOG: Error creating render context
				DestroyContext ();
				return false;
			}

			// make it the current render context
			wglMakeCurrent( m_deviceContext, m_renderContext );

			return true;
		}

		void ContextOpenGL::DestroyContext()
		{
			if ( m_renderContext ) {
				wglMakeCurrent( NULL, NULL );
				wglDeleteContext( m_renderContext );
			}
			if ( m_windowHandle && m_deviceContext ) {
				ReleaseDC( m_windowHandle, m_deviceContext );
			}
			ResetHandles();
		}

		void ContextOpenGL::ResetHandles()
		{
			m_windowHandle = NULL;
			m_deviceContext = NULL;
			m_renderContext = NULL;
		}

		bool ContextOpenGL::SwapWindowBuffers()
		{
			//More info on Swap: http://msdn.microsoft.com/en-us/library/windows/desktop/dd369060(v=vs.85).aspx
			//Removing performance warning for the conversion from BOOL to bool ( or smth)
#pragma warning( disable:4800)
			return SwapBuffers ( m_deviceContext );
#pragma warning( default:4800)
		}

	};

};

#endif // system_win32_impl_opengl_h__
