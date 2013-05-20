#ifndef GLVertexBufferFwd_h__
#define GLVertexBufferFwd_h__

#include "Core/SmartPointers.h"
namespace Bolts {
	namespace Rendering {

		class GLVertexBuffer;
		typedef IntrusivePtr< GLVertexBuffer>::t GLVertexBufferPtr;
		typedef IntrusivePtr< const GLVertexBuffer>::t GLVertexBufferCPtr;
	};
};
#endif // GLVertexBufferFwd_h__