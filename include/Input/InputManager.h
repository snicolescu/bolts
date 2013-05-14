#ifndef InputManager_h__
#define InputManager_h__

#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISJoyStick.h>
#include <OIS/OISInputManager.h>

namespace Bolts {

	class InputManager : public OIS::KeyListener, OIS::MouseListener, OIS::JoyStickListener {
		public:
			InputManager();
			~InputManager();

			void init( size_t windowHandle, int windowWidth, int windowHeight );
			void capture( void );

			void addKeyListener( OIS::KeyListener *keyListener, const std::string &instanceName );
			void addMouseListener( OIS::MouseListener *mouseListener, const std::string &instanceName );
			void addJoystickListener( OIS::JoyStickListener *joystickListener, const std::string &instanceName );

			void removeKeyListener( const std::string &instanceName );
			void removeMouseListener( const std::string &instanceName );
			void removeJoystickListener( const std::string &instanceName );

			void removeKeyListener( OIS::KeyListener *keyListener );
			void removeMouseListener( OIS::MouseListener *mouseListener );
			void removeJoystickListener( OIS::JoyStickListener *joystickListener );

			void removeAllListeners( void );
			void removeAllKeyListeners( void );
			void removeAllMouseListeners( void );
			void removeAllJoystickListeners( void );

			void setWindowExtents( int width, int height );

			int getNumOfJoysticks( void );

		private:
			bool keyPressed( const OIS::KeyEvent &e );
			bool keyReleased( const OIS::KeyEvent &e );

			bool mouseMoved( const OIS::MouseEvent &e );
			bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
			bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

			bool povMoved( const OIS::JoyStickEvent &e, int pov );
			bool axisMoved( const OIS::JoyStickEvent &e, int axis );
			bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID );
			bool buttonPressed( const OIS::JoyStickEvent &e, int button );
			bool buttonReleased( const OIS::JoyStickEvent &e, int button );

			OIS::InputManager *mInputSystem;
			OIS::Mouse        *mMouse;
			OIS::Keyboard     *mKeyboard;

			std::vector<OIS::JoyStick *> mJoysticks;
			std::vector<OIS::JoyStick *>::iterator itJoystick;
			std::vector<OIS::JoyStick *>::iterator itJoystickEnd;

			std::map<std::string, OIS::KeyListener *> mKeyListeners;
			std::map<std::string, OIS::MouseListener *> mMouseListeners;
			std::map<std::string, OIS::JoyStickListener *> mJoystickListeners;

			std::map<std::string, OIS::KeyListener *>::iterator itKeyListener;
			std::map<std::string, OIS::MouseListener *>::iterator itMouseListener;
			std::map<std::string, OIS::JoyStickListener *>::iterator itJoystickListener;

			std::map<std::string, OIS::KeyListener *>::iterator itKeyListenerEnd;
			std::map<std::string, OIS::MouseListener *>::iterator itMouseListenerEnd;
			std::map<std::string, OIS::JoyStickListener *>::iterator itJoystickListenerEnd;
	};

};
#endif // InputManager_h__