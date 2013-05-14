#pragma once
#include <gl/glew.h>
#include <gl/GL.h>

#include "BSmartPointers.h"
#include "BLogger.h"
#include "BShader.h"
#include <string>

namespace Bolts {
	namespace Rendering {

		class GLSLShader: public tiny_ref_counted_type {
			public:
				GLSLShader(): mShaderIndex( 0 ) {};

				const bool loadedAndOK() const {
					return ( mShaderIndex != 0 );
				};
				GLint getShaderIndex() const {
					return mShaderIndex;
				};

				//Loads the shader code and compiles it.
				//Please note that it will also ask for a new shader program from OGL.
				//Returns false if the operation failed
				bool loadShader( ShaderType stype, const std::string &code );

				~GLSLShader();
			private:
				GLint mShaderIndex;

				bool loadIndex( ShaderType stype );
				bool loadCode( const std::string &code );

				GLuint getShaderConstant( ShaderType stype );
		};

	};
};