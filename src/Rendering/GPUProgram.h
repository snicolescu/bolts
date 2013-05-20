#ifndef GPUProgram_h__
#define GPUProgram_h__

#ifdef BOLTS_OPENGL
#include "Rendering/GL/GLSLProgram.h"

namespace Bolts {
	namespace Rendering {
		typedef GLSLProgram			GPUProgram;
		typedef GLSLProgramPtr		GPUProgramPtr;
	};
};
#endif

#endif // GPUProgram_h__