#include "BGLSLProgramBuilder.h"

using namespace Bolts;
using namespace Bolts::Rendering;

bool GLSLProgramBuilder::readyToCompile()
{
	return ( mShaderList.size() > 0 );
}

bool GLSLProgramBuilder::buildProgram( ProgramReference prog, DescriptionReference desc )
{
	if ( !readyToCompile() ) {
		BLogger::error( "[ProgramBuilder] Asked to compile blank list of shaders" );
		return false;
	}

	prog.deleteProgram();
	prog.mProgramIndex = glCreateProgram();
	if ( prog.mProgramIndex == 0 ) {
		BLogger::error( "[ProgramBuilder] Error creating program index" );
		return false;
	}

	for ( auto i = mShaderList.begin(); i != mShaderList.end(); ++i ) {
		glAttachShader( prog.mProgramIndex, ( *i )->getShaderIndex() );
	}

	//Attach attributes to inputs
	bindAttributes( prog.mProgramIndex );
	// Attempt to link
	glLinkProgram( prog.mProgramIndex );

	GLint val;
	glGetProgramiv( prog.mProgramIndex, GL_LINK_STATUS, &val );
	if  ( val == GL_FALSE ) {
		//Linking failed
		char infoLog[1024];
		glGetProgramInfoLog( prog.mProgramIndex, 1024, NULL, infoLog );
		BLogger::error( std::string( "[ProgramBuilder] Program linking failed:" ) );
		BLogger::print( std::string( infoLog ) );
		prog.deleteProgram();
		return false;
	}

	readUniforms( prog, desc );
	return true;
}

void GLSLProgramBuilder::bindAttributes( GLuint progIndex )
{
	//Bind attributes to their predifined locations
	for ( uint_ i = 0; i < _BSA_COUNT; ++i ) {
		glBindAttribLocation( progIndex, i, AttributeBindingNames[i] );
	}
}

void GLSLProgramBuilder::readUniforms( ProgramReference prog, DescriptionReference desc )
{
	//Get the number of active uniforms
	GLint numUniforms;
	glGetProgramiv( prog.mProgramIndex, GL_ACTIVE_UNIFORMS, &numUniforms );
	GLuint uniformIndices[MAX_ACTIVE_UNIFORMS];
	if ( numUniforms > MAX_ACTIVE_UNIFORMS ) {
		numUniforms = MAX_ACTIVE_UNIFORMS;
	}
	for ( ushort_ i = 0; i < numUniforms; ++i ) {
		uniformIndices[i] = i;
	}

	//First, the builtins
	prog.mBuiltins.clear();
	GLint uniformLocation;
	std::array< GLint , MAX_ACTIVE_UNIFORMS > builtinLocationsUsed;
	builtinLocationsUsed.fill( 0 );

	for( uint_ i = 0; i < _BSP_COUNT; ++i ) {
		uniformLocation = glGetUniformLocation( prog.mProgramIndex, BuiltinParameterNames[i] );
		if ( uniformLocation != -1 ) {
			prog.mBuiltins[ ( BuiltinParameter )i] = uniformLocation;
			//And take this index out of the custom uniform list
			uniformIndices[ uniformLocation] = GL_INVALID_INDEX;
			desc.addBuiltinRequirement( ( BuiltinParameter )i );
		}
	}

	//And now the custom uniforms
	prog.mCustomParameters.clear();
	//And their data types
	GLint uniformTypes[MAX_ACTIVE_UNIFORMS];
	GLint uniformSizes[MAX_ACTIVE_UNIFORMS];
	glGetActiveUniformsiv( prog.mProgramIndex, numUniforms, uniformIndices, GL_UNIFORM_TYPE, uniformTypes );
	glGetActiveUniformsiv( prog.mProgramIndex, numUniforms, uniformIndices, GL_UNIFORM_SIZE, uniformSizes );
	//Get the name of each active uniform and add it to the map
	char name[LONGEST_UNIFORM_NAME];
	for ( GLuint i = 0; i < numUniforms; ++i ) {
		if ( uniformIndices[i] != GL_INVALID_INDEX ) {
			glGetActiveUniformName( prog.mProgramIndex, i, LONGEST_UNIFORM_NAME, NULL, name );

			GLSLProgram::UniformDescription udesc = { uniformTypes[i], uniformSizes[i], i};
			prog.mCustomParameters[ std::string( name )] = udesc;
			ShaderDescription::ParameterDescription pdesc = { glParamToShaderParam( uniformTypes[i] ), uniformSizes[i]};
			desc.addParameterDefinition( std::string( name ), pdesc );
		}
	}
}

void GLSLProgramBuilder::addShader( ShaderPointer sp )
{
	if ( sp->loadedAndOK() ) {
		mShaderList.push_back( sp );
	}
}

ShaderParamType GLSLProgramBuilder::glParamToShaderParam( GLint glparam )
{
	switch( glparam ) {
		case GL_FLOAT:
			return BST_FLOAT;
		case GL_FLOAT_VEC2:
			return BST_VEC2;
		case GL_FLOAT_VEC3:
			return BST_VEC3;
		case GL_FLOAT_VEC4:
			return BST_VEC4;
		case GL_FLOAT_MAT2:
			return BST_MAT2;
		case GL_FLOAT_MAT3:
			return BST_MAT3;
		case GL_FLOAT_MAT4:
			return BST_MAT4;
		case GL_INT:
			return BST_INT;
		case GL_UNSIGNED_INT:
			return BST_UINT;
		case GL_BOOL:
			return BST_BOOL;
		default:
			//TODO: Report that the shader doesn't know parameters of that type.
			return BST_UNKNOWNTYPE;
	}
}