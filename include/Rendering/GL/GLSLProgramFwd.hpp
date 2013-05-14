#ifndef GLSLProgramFwd_h__
#define GLSLProgramFwd_h__

#include "Core/types.h"
#include "Core/SmartPointers.h"

namespace Bolts {
	namespace Rendering {

		class GLSLProgram;
		typedef IntrusivePtr< GLSLProgram>::t GLSLProgramPtr;
		typedef IntrusivePtr< const GLSLProgram>::t GLSLProgramCPtr;
	};
};

#endif // GLSLProgramFwd_h__