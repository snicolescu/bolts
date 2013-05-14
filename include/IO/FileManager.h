#pragma once

namespace Bolts {
	namespace IO {

		//Returns the number of characters read.
		size_t readFromFileAsText( const char *filename, char *textBuffer, size_t textBufferLen );
		size_t fileSize( const char *filename );
	};
};