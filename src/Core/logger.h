#pragma once
#include <string>
#include <iostream>
#include "loki/Singleton.h"

#include <Core\types.h>

#ifndef BOLTS_LOG_MAX_LEVEL
#define BOLTS_LOG_MAX_LEVEL Log::Level::VERBOSE
#endif

namespace Bolts {

	class Log {
		public:
			typedef uint16_t	channelID_t;
			enum Level { ERROR = 0, WARNING, INFO, VERBOSE};
			//Returns the ID of the new channel
			//NOTE: Channels cannot be removed
			channelID_t		AddChannel( const std::string &channelName );
			//Returns previously set value
			channelID_t		SetChannel( channelID_t channelID );
			//Returns the previously set level
			uint8_t			SetLevel( uint8_t logLevel );

		private:
			typedef std::vector< std::string>	chNameCont_t;

			chNameCont_t	m_channelNames;
			channelID_t		m_defaultChannel;
			uint8_t			m_defaultLevel;
	};

	namespace Console {

		template< uint8_t Lev>
		struct Level {
			static const uint8_t value = Lev;
		};
		enum Flag { F_WARN = 1, F_ERR = 2, F_HINT = 1 << 2};
		enum Color { C_BLACK = 0, C_BLUE, C_GREEN, C_RED = 0xC, C_YELLOW = 6, C_WHITE, C_GRAY};
		enum Endl {ENDL};
	};

	class ConsoleLog {
		public:
			ConsoleLog(): m_enabledFlags( 7 ) {};
			~ConsoleLog();

			ConsoleLog &operator<<( Console::Flag );
			ConsoleLog &operator<<( Console::Color );
			ConsoleLog &operator<<( Console::Endl );
			//TODO: Logger specific functions for small types
			/*	Log& operator << ( int value);
				Log& operator << ( bool value);
				Log& operator << ( float value);
			*/

			ConsoleLog &operator << ( const char * );

			template< typename Val >
			ConsoleLog &operator<< ( const Val &value ) {
				std::cout << value;
				return *this;
			}

		private:
			void Reset();

			uint8_t m_enabledFlags;
	};

	typedef class Loki::SingletonHolder <
		ConsoleLog,
		Loki::CreateUsingNew,
		Loki::DefaultLifetime
			> LogStn;

#define LOGINST LogStn::Instance()

};