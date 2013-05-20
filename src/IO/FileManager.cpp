#include "IO/FileManager.h"
#include <fstream>
#include <iostream>

size_t Bolts::IO::readFromFileAsText( const char *filename, char *textBuffer, size_t textBufferLen )
{
	std::ifstream file;
	file.open( filename, std::ifstream::in );
	if ( file.fail() ) {
		std::cout << " ------- Error opening file: " << filename << std::endl;
		return 0;
	}

	file.read( textBuffer, textBufferLen );
	std::streamsize read = file.gcount();
	textBuffer[ read ] = '\0';
	return ( size_t ) read;
}

size_t Bolts::IO::fileSize( const char *filename )
{
	std::ifstream in( filename, std::ifstream::in | std::ifstream::binary );
	if ( in.fail() ) {
		std::cout << " ------- Error opening file: " << filename << std::endl;
		return 0;
	}
	in.seekg( 0, std::ifstream::end );
	return ( size_t ) in.tellg();
}