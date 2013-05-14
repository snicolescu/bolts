#pragma once
#include <gl/glew.h>
#include <gl/GL.h>

#include "BTypes.h"
#include "BSmartPointers.h"
#include "BLogger.h"
#include "BShader.h"
#include "BGLSLShader.h"
#include "BGLSLProgram.h"
#include <string>
#include <list>
#include <map>
#include <array>

namespace Bolts {
	namespace Rendering {
		const ushort_ MAX_ACTIVE_UNIFORMS = 32;
		const ushort_ LONGEST_UNIFORM_NAME = 64;

		//This class builds Shader Programs and Descriptions from its list of shaders
		class GLSLProgramBuilder {
			public:
				struct UniformDescription {
					GLint mUniformType;
					ushort_ mArraySize;
					GLuint uniformIndex;
				};
				typedef typename intrusive_ptr< const GLSLShader >::t ShaderPointer;
				typedef ShaderDescription &DescriptionReference;
				typedef GLSLProgram &ProgramReference;

				GLSLProgramBuilder(): mShaderList() {};

				void addShader( ShaderPointer shader );
				void removeShader( ShaderPointer shader );

				//Creates a new GLSL Program and adds it's data into prog, and it's descriptor in desc
				//Returns false on failure, and logs the reason.
				bool buildProgram( ProgramReference prog, DescriptionReference desc );

			private:
				std::list< ShaderPointer > mShaderList;

				bool readyToCompile();
				void bindAttributes( GLuint programIndex );
				void readUniforms( ProgramReference prog, DescriptionReference desc );

				ShaderParamType glParamToShaderParam( GLint glparam );
		};
	};
};