#include "Rendering/GL/GLSLProgram.h"
#include "Rendering/GL/GLDriver.h"
#include <iostream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

Bolts::Rendering::GLSLShader Bolts::Rendering::LoadShader( const char *shaderCode, size_t codeSizeInBytes, GLenum shaderType )
{
	Bolts::Rendering::GLSLShader shader;
	shader.m_shaderID = glCreateShader( shaderType );
	shader.m_shaderType = shaderType;
	if ( shader.m_shaderID == 0 ) {
		std::cout << " ------- Error creating shader" << std::endl;
		return shader;
	}

	glShaderSource( shader.m_shaderID, 1, &shaderCode, ( GLint * ) &codeSizeInBytes );
	glCompileShader( shader.m_shaderID );
	GLint result;
	glGetShaderiv( shader.m_shaderID, GL_COMPILE_STATUS, &result );

	if ( result == GL_FALSE ) {
		std::cout << " ------- Error compiling shader: " << std::endl;

		glGetShaderiv( shader.m_shaderID, GL_INFO_LOG_LENGTH, &result );
		char *infoLog = new char[ result];
		glGetShaderInfoLog( shader.m_shaderID, result, NULL, infoLog );
		std::cout << infoLog << std::endl;

		glDeleteShader( shader.m_shaderID );
		shader.m_shaderID = 0;
		delete[] infoLog;
		return shader;
	}

	return shader;
}

Bolts::Rendering::GLSLProgramPtr Bolts::Rendering::GLSLProgram::Create()
{
	Bolts::Rendering::GLSLProgramPtr program = new Bolts::Rendering::GLSLProgram();
	program->m_programID = glCreateProgram();
	return program;
}

Bolts::Rendering::GLSLProgram::~GLSLProgram()
{
	if ( m_programID != 0 ) {
		glDeleteProgram( m_programID );
	}
}

void Bolts::Rendering::GLSLProgram::AttachShader( Bolts::Rendering::GLSLShader shader )
{
	if ( m_programID == 0 ) {
		return;
	}
	glAttachShader( m_programID, shader.m_shaderID );
}

void Bolts::Rendering::GLSLProgram::Enable()
{
	if ( !m_ready && !Link() ) {
		return;
	}
	m_enabled = true;
	glUseProgram( m_programID );
}

bool Bolts::Rendering::GLSLProgram::Link()
{
	if ( m_programID == 0 ) {
		//TODO: Log here
		std::cout << " ------- Error linking shading program: not a valid program ID 0" << std::endl;
		return false;
	}

	GLDriverStn::Instance ().SetDefaultAttributePositions ( m_programID );
	glLinkProgram( m_programID );

	GLint linked;
	glGetProgramiv( m_programID, GL_LINK_STATUS, &linked );
	if ( !linked ) {
		std::cout << " ------- Error linking shading program: " << std::endl;

		glGetProgramiv( m_programID, GL_INFO_LOG_LENGTH, &linked );
		char *infoLog = new char[ linked];
		glGetProgramInfoLog( m_programID, linked, NULL, infoLog );
		std::cout << infoLog << std::endl;
		delete[] infoLog;
		m_ready = false;
	} else {
		m_ready = true;

		LoadParameterInfo();
	}

	return m_ready;
}

void Bolts::Rendering::GLSLProgram::PrintParameterInfo()
{
	if ( !m_ready ) {
		return;
	}

	std::cout << " ------- Program Info LOG -------" << std::endl;

	std::cout << " Active Attributes: " << ( int ) m_numAttributes << std::endl;

	std::cout << " Active Uniforms: " << ( int ) m_numUniforms << std::endl;

	std::cout << " -- Attributes --" << std::endl << std::endl;
	for ( int i = 0; i < m_numAttributes; ++i ) {
		std::cout << " " << i << " - \"" << m_attributes[i].paramName << "\" Size: " << m_attributes[i].paramSize << " Type: " << m_attributes[i].paramType << std::endl;
	}

	std::cout << std::endl << " -- Uniforms --" << std::endl << std::endl;
	for ( int i = 0; i < m_numUniforms; ++i ) {
		std::cout << " " << i << " - \"" << m_uniforms[i].paramName << "\" Size: " << m_uniforms[i].paramSize << " Type: " << m_uniforms[i].paramType << std::endl;
	}

	std::cout << std::endl;
}

void Bolts::Rendering::GLSLProgram::LoadParameterInfo()
{
	if ( !m_ready ) {
		return;
	}

	GLint value;

	glGetProgramiv( m_programID, GL_ACTIVE_ATTRIBUTES, &value );
	m_numAttributes = ( uint8_t ) value;
	m_attributes.reserve ( m_numAttributes );
	glGetProgramiv( m_programID, GL_ACTIVE_UNIFORMS, &value );
	m_numUniforms = ( uint16_t ) value;
	m_uniforms.reserve ( m_numUniforms );

	int longestAttr = 0, longestUniform = 0;
	glGetProgramiv( m_programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &value );
	longestAttr = value;
	glGetProgramiv( m_programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &value );
	longestUniform = value;

	char *nameHolder = new char[ longestAttr ];
	{
		AttribDesc desc;
		for ( int i = 0; i < m_numAttributes; ++i ) {
			glGetActiveAttrib( m_programID, i, longestAttr, NULL, &desc.paramSize, &desc.paramType, nameHolder );
			desc.paramName = nameHolder;
			m_attributes.push_back( desc );
		}
	}
	delete[] nameHolder;
	std::sort( m_attributes.begin (), m_attributes.end () );

	nameHolder = new char[ longestUniform ];
	{
		UniformDesc desc;
		for ( uint16_t i = 0; i < m_numUniforms; ++i ) {
			glGetActiveUniform( m_programID, i, longestUniform, NULL, &desc.paramSize, &desc.paramType, nameHolder );
			desc.paramIndex = i;
			desc.paramName = nameHolder;
			m_uniforms.push_back( desc );
		}
	}
	delete[] nameHolder;
	std::sort( m_uniforms.begin (), m_uniforms.end () );
}

bool Bolts::Rendering::GLSLProgram::SetUniformValue( const UniformDesc &uniform, float newValue )
{
	BOOST_ASSERT_MSG( m_enabled, "ERROR: Program must be enabled for uniform to be set" );

	if ( uniform.paramType != GL_FLOAT ) {
		//TODO LOG: Attempting to set uniform value of type uniform.paramType from a float
		return false;
	}

	glUniform1f( uniform.paramIndex, newValue );

	return true;
}

bool Bolts::Rendering::GLSLProgram::SetUniformValue( const UniformDesc &uniform, const vec2 &newValue )
{
	BOOST_ASSERT_MSG( m_enabled, "ERROR: Program must be enabled for uniform to be set" );
	if ( uniform.paramType != GL_FLOAT_VEC2 ) {
		//TODO LOG: Attempting to set uniform value of type uniform.paramType from a float
		return false;
	}
	glUniform2f( uniform.paramIndex, newValue.x, newValue.y );

	return true;
}

bool Bolts::Rendering::GLSLProgram::SetUniformValue( const UniformDesc &uniform, const vec3 &newValue )
{
	BOOST_ASSERT_MSG( m_enabled, "ERROR: Program must be enabled for uniform to be set" );
	if ( uniform.paramType != GL_FLOAT_VEC3 ) {
		//TODO LOG: Attempting to set uniform value of type uniform.paramType from a float
		return false;
	}
	glUniform3f( uniform.paramIndex, newValue.x, newValue.y, newValue.z );

	return true;
}

bool Bolts::Rendering::GLSLProgram::SetUniformValue( const UniformDesc &uniform, const vec4 &newValue )
{
	BOOST_ASSERT_MSG( m_enabled, "ERROR: Program must be enabled for uniform to be set" );
	if ( uniform.paramType != GL_FLOAT_VEC4 ) {
		//TODO LOG: Attempting to set uniform value of type uniform.paramType from a float
		return false;
	}
	glUniform4f( uniform.paramIndex, newValue.x, newValue.y, newValue.z, newValue.w );

	return true;
}

bool Bolts::Rendering::GLSLProgram::SetUniformValue( const UniformDesc &uniform, const mat4 &newValue )
{
	BOOST_ASSERT_MSG( m_enabled, "ERROR: Program must be enabled for uniform to be set" );
	if ( ( uniform.paramType != GL_FLOAT_MAT4 ) ) {
		//TODO LOG: Attempting to set uniform value of type uniform.paramType from a float
		return false;
	}
	glUniformMatrix4fv( uniform.paramIndex, 1, 0, glm::value_ptr( newValue ) );

	return true;
}
