#ifndef GPUProgramFwd_h__
#define GPUProgramFwd_h__

#ifdef BOLTS_OPENGL
#include "Rendering/GL/GLSLProgramFwd.hpp"

namespace Bolts {
	namespace Rendering {
		typedef GLSLProgram			GPUProgram;
		typedef GLSLProgramPtr		GPUProgramPtr;
	};
};
#endif

#endif // GPUProgramFwd_h__