#include "Core/logger.h"

#ifdef WIN32
#include <windows.h>
#endif

Bolts::ConsoleLog::~ConsoleLog()
{
	Reset();
}

Bolts::ConsoleLog &Bolts::ConsoleLog::operator<<( Bolts::Console::Color col )
{
#ifdef WIN32
	HANDLE hConsole;
	hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( hConsole, ( WORD ) col );
#endif
	return *this;
}

Bolts::ConsoleLog &Bolts::ConsoleLog::operator<<( Bolts::Console::Endl )
{
	std::cout << std::endl;
	Reset ();
	return *this;
}

Bolts::ConsoleLog &Bolts::ConsoleLog::operator<<( const char *txt )
{
	std::cout << txt;
	return *this;
}

Bolts::ConsoleLog &Bolts::ConsoleLog::operator<<( Bolts::Console::Flag flag )
{
	if ( ( m_enabledFlags & flag ) == Bolts::Console::F_WARN ) {
		*this << Bolts::Console::C_YELLOW;
		return *this << "[Warning] ";
	}
	if ( ( m_enabledFlags & flag ) == Bolts::Console::F_ERR ) {
		*this << Bolts::Console::C_RED;
		return *this << "[Error]   ";
	}
	if ( ( m_enabledFlags & flag ) == Bolts::Console::F_HINT ) {
		return *this << "[Hint]    ";
	}
	return *this;
}

void Bolts::ConsoleLog::Reset()
{
#ifdef WIN32
	HANDLE hConsole;
	hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	//Set Color back to white
	SetConsoleTextAttribute( hConsole, Bolts::Console::C_WHITE );
#endif
}


