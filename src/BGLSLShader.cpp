#include "BGLSLShader.h"

using namespace Bolts;
using namespace Bolts::Rendering;

GLuint GLSLShader::getShaderConstant( ShaderType stype )
{
	switch ( stype ) {
		case BS_VERTEX:
			return GL_VERTEX_SHADER;
		case BS_FRAGMENT:
			return GL_FRAGMENT_SHADER;
		case BS_GEOMETRY:
			return GL_GEOMETRY_SHADER;
		default:
			return 0;
	}
};

bool GLSLShader::loadShader( ShaderType stype, const std::string &code )
{
	if ( loadIndex( stype ) && loadCode( code ) ) {
		return true;
	} else {
		return false;
	}
};

bool GLSLShader::loadIndex( ShaderType stype )
{
	if ( mShaderIndex != 0 ) {
		glDeleteShader( mShaderIndex );
	}

	mShaderIndex = glCreateShader( getShaderConstant( stype ) );
	if ( mShaderIndex == 0 ) {
		BLogger::error( "Error creating shader index" );
		return false;
	}
};

bool GLSLShader::loadCode( const std::string &code )
{
	GLint length = code.size();
	const GLchar *codePointer = code.c_str();
	glShaderSource( mShaderIndex, 1, &codePointer, &length );

	glCompileShader( mShaderIndex );
	GLint val;
	glGetShaderiv( mShaderIndex, GL_COMPILE_STATUS, &val );
	if  ( val == GL_FALSE ) {
		char infoLog[1024];
		glGetShaderInfoLog( mShaderIndex, 1024, NULL, infoLog );
		BLogger::error( std::string( "Shader compilation failed:" ) );
		BLogger::print( std::string( infoLog ) );
		return false;
	}
	return true;
};

GLSLShader::~GLSLShader()
{
	if ( mShaderIndex != 0 ) {
		glDeleteShader( mShaderIndex );
	}
};