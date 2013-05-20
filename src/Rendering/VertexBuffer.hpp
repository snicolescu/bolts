#ifndef VertexBuffer_h__
#define VertexBuffer_h__

#ifdef BOLTS_OPENGL
#include "Rendering/GL/GLVertexBuffer.h"

namespace Bolts {
	namespace Rendering {

		typedef GLVertexBuffer		VertexBuffer; 
		typedef GLVertexBufferPtr	VertexBufferPtr;
		typedef GLVertexBufferCPtr	VertexBufferCPtr;
	};
};
#endif

#endif // VertexBuffer_h__