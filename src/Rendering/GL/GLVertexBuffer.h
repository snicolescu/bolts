#ifndef GLVertexBuffer_h__
#define GLVertexBuffer_h__

#include "glew/GL/glew.h"
#include "Rendering/Common/GPUVertexBufferBase.h"
#include <Rendering/GL/GLVertexBufferFwd.hpp>

namespace Bolts {
	namespace Rendering {

		class GLDriver;

		//TODO PERF: Add flag for determining when you are updating data on the GPU
		//			Special care must be taken if the buffer is still in use
		class GLVertexBuffer: public GPUVertexBufferBase {
			public:
				friend class GLDriver;

				GLVertexBuffer();
				GLVertexBuffer( const GPUBufferDescription & );
				~GLVertexBuffer();

				bool operator== ( const GLVertexBuffer &other ) {
					return ( m_bufferID != other.m_bufferID );
				}

				bool HasID() const;

			private:
				void FreeID();
				void UploadData() const;

				GLuint	m_bufferID;
		};

	};
};
#endif // GLVertexBuffer_h__