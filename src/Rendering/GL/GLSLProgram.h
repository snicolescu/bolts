#pragma once
#include "glew/GL/glew.h"
#include <vector>
#include <string>

#include "Core/types.h"
#include "Core/math_types.h"
#include "Core/SmartPointers.h"
#include "Rendering/GL/GLSLProgramFwd.hpp"

namespace Bolts {
	namespace Rendering {

		struct GLSLShader {
			GLSLShader(): m_shaderID( 0 ), m_shaderType() {}

			bool IsValid() {
				return ( m_shaderID != 0 );
			}

			GLuint m_shaderID;
			GLenum m_shaderType;
		};

		//TODO: Move this somewhere appropriate
		GLSLShader LoadShader( const char *shaderCode, size_t codeSizeInBytes, GLenum shaderType );

		//TODO: Keep track of shaders, so you can properly destroy them as well.
		//TODO: Get valid program ID in Link()
		class GLSLProgram: public SmallRefCountT {
			public:
				struct AttribDesc {
					std::string paramName;
					GLenum		paramType;
					GLint		paramSize; //Number of elements( for vectors/matrices)
					uint16_t		paramIndex;

					//Comparison operator defined for sorting containers
					bool operator< ( const AttribDesc &other ) {
						return paramName < other.paramName;
					}
				};

				struct UniformDesc: public AttribDesc {
				};

				typedef GLSLShader					ShaderType;

				typedef std::vector< AttribDesc >	attributeList_t;
				typedef std::vector< UniformDesc >	uniformList_t;

				GLSLProgram(): m_programID( 0 ), m_ready( false ), m_enabled( false ), m_uniforms(), m_attributes() {}
				~GLSLProgram();

				//TODO: Remove this
				static GLSLProgramPtr		Create();

				bool operator== ( const GLSLProgram &other ) {
					return m_programID != other.m_programID;
				}
				bool operator!= ( const GLSLProgram &other ) {
					return !( *this == other );
				}

				void					AttachShader( GLSLShader shader );

				//Returns true on compilation success
				bool					Link();

				//Make program available for rendering
				void					Enable();
				const attributeList_t	&GetUsedAttributes() {
					return m_attributes;
				}
				const uniformList_t	&GetUsedUniforms() {
					return m_uniforms;
				}

				//These return false if the value wasn't set properly
				bool					SetUniformValue( const UniformDesc &uniform, float newValue );
				bool					SetUniformValue( const UniformDesc &uniform, const vec2 &newValue );
				bool					SetUniformValue( const UniformDesc &uniform, const vec3 &newValue );
				bool					SetUniformValue( const UniformDesc &uniform, const vec4 &newValue );
				bool					SetUniformValue( const UniformDesc &uniform, const mat4 &newValue );

				void					PrintParameterInfo();

				bool					IsReady() {
					return m_ready;
				}

			private:
				void					LoadParameterInfo();



				attributeList_t		m_attributes;
				uniformList_t		m_uniforms;
				GLuint				m_programID;

				uint8_t				m_numAttributes;
				uint16_t			m_numUniforms;
				bool				m_ready;
				bool				m_enabled;
		};

	};
};