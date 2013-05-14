#ifndef VertexBufferFwd_h__
#define VertexBufferFwd_h__

#ifdef BOLTS_OPENGL
#include "Rendering/GL/GLVertexBufferFwd.hpp"

namespace Bolts {
	namespace Rendering {

		typedef GLVertexBuffer		VertexBuffer; 
		typedef GLVertexBufferPtr	VertexBufferPtr;
		typedef GLVertexBufferCPtr	VertexBufferCPtr;
	};
};
#endif

#endif // VertexBufferFwd_h__