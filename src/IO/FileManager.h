#pragma once

#include "Core/types.h"
#include "Core/SmartPointers.h"
#include <vector>

namespace Bolts {
	namespace IO {

		class IFilePtr;

		class IFile: public BigRefCountT {
			public:

				virtual bool		IsOpen() {
					return m_IsOpen;
				};
				virtual uint64_t	ReadData( void *outBuffer, uint64_t bufferSize ) = 0;
				virtual uint64_t	ReadData( std::vector<uint8_t> &outBuffer ) = 0;
			protected:
				bool	m_IsOpen;

		};

		class IFileArchive: public SmallRefCountT {
			public:
				virtual IFilePtr	OpenFile( const std::string &filepath, bool ignorePath ) = 0;
		};

		typedef IntrusivePtr<IFileArchive>::t	IFileArchivePtr;

		class FileSystem {
			public:
				void		OpenArchive( const std::string &archivePath );
				void		AddArchive( IFileArchivePtr archive );
			private:

		};



		//Returns the number of characters read.
		size_t readFromFileAsText( const char *filename, char *textBuffer, size_t textBufferLen );
		size_t fileSize( const char *filename );
	};
};