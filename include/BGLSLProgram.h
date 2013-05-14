#pragma once
#include <gl/glew.h>
#include <gl/GL.h>

#include "BTypes.h"
#include "BShader.h"
#include <string>
#include <map>

namespace Bolts {
	namespace Rendering {
		//This class will hold the data required by the OGLWrapper to bind the received material parameters to the proper places.
		//TODO: Add texture info and support
		struct GLSLProgram {
			struct UniformDescription {
				GLint mUniformType;
				ushort_ mArraySize;
				GLuint uniformIndex;
			};
			typedef std::map< BuiltinParameter, GLint> BuiltinList;
			typedef std::map< std::string, UniformDescription> ParameterList;

			GLuint mProgramIndex;
			BuiltinList mBuiltins;
			ParameterList mCustomParameters;

			void deleteProgram() {
				if ( mProgramIndex != 0 ) {
					glDeleteProgram( mProgramIndex );
					mProgramIndex = 0;
				}
			};
		};
	};
};