#include "BBaseRenderer.h"

using namespace Bolts;

template <class LLW>
void BBaseRenderer<LLW>::init( int width, int height )
{

	mLLW.init( width, height );

	viewportChanged( width, height );

	mShaderManager.InitializeStockShaders();
	mTransformPipeline.SetModelViewMatrixStack( mModelViewMatrix );

	initShaders();
}

template< class LLW>
void BBaseRenderer<LLW>::viewportChanged( int width, int height )
{
	mLLW.viewportChanged( width, height );

	// Create the projection matrix, and load it on the projection matrix stack
	mViewFrustum.SetPerspective( 35.0f, float( width ) / float( height ), 1.0f, 100.0f );
	mProjectionMatrix.LoadMatrix( mViewFrustum.GetProjectionMatrix() );

	// Set the transformation pipeline to use the two matrix stacks
	mTransformPipeline.SetProjectionMatrixStack( mProjectionMatrix );
}

template<class LLW>
void BBaseRenderer<LLW>::render()
{
	mModelViewMatrix.PushMatrix( glm::value_ptr( mViewMatrix ) );

	mModelViewMatrix.PopMatrix();
}

template<class LLW>
void BBaseRenderer<LLW>::loadMesh( RIDType mid, const BMesh &mesh )
{
	//TODO: Error checking (see if buffers were actually created, etc.
	MeshInfo &minfo = mMeshData[mid];
	minfo.hasIndexData = mesh.hasIndexData();
	BMesh::DataInfo mDataInfo = mesh.getDataInfo();

	switch( mesh.getPrimitiveType() ) {
		case BMesh::B_MP_LINES:
			minfo.primitiveType = GL_LINES;
			break;
		case BMesh::B_MP_TRIANGLES:
			minfo.primitiveType = GL_TRIANGLES;
			break;
		case BMesh::B_MP_TRIANGLESTRIP:
			minfo.primitiveType = GL_TRIANGLE_STRIP;
			break;
	}

	if ( minfo.hasIndexData ) {
		minfo.numVertices = mesh.getNumIndices();
	} else {
		minfo.numVertices = mesh.getNumVertices();
	}

	glGenVertexArrays( 1, &minfo.vao );

	minfo.vertexBuffers.resize( mDataInfo.buffersUsed );
	byte_t nextBuff = 0;
	glGenBuffers( mDataInfo.buffersUsed, &minfo.vertexBuffers[0] );
	//Index data will be stored in the last buffer

	glBindVertexArray( minfo.vao );

	if ( mesh.hasIndexData() ) {
		const std::vector< uint_ > &indexData = mesh.getIndexData();
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, minfo.vertexBuffers[ mDataInfo.buffersUsed - 1 ] );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof( uint_ ), &indexData[0] , GL_STATIC_DRAW );
	}

	for ( int i = 0; i < mDataInfo.hasData.size() ; ++i ) {
		if ( mDataInfo.hasData[i] ) {
			const std::vector< float > &verData = mesh.getData( ( BMesh::B_MESH_DATA ) i );
			glBindBuffer( GL_ARRAY_BUFFER, minfo.vertexBuffers[nextBuff] );
			glBufferData( GL_ARRAY_BUFFER, verData.size() * sizeof( float ), &verData[0], GL_STATIC_DRAW );
			glVertexAttribPointer( i, BE_SHADER_DATA_SIZE[i] , GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray( i );
			nextBuff++;
		}
	}

	glBindVertexArray( 0 );
}

void BOpenGLRenderer::loadMesh( RIDType mid, const BObjMesh &mesh )
{
	//TODO: Error checking (see if buffers were actually created, etc.
	MeshInfo &minfo = mMeshData[mid];
	minfo.hasIndexData = true;
	minfo.numVertices = mesh.mFaces.size() * 3;

	glGenVertexArrays( 1, &minfo.vao );

	int buffersUsed = 2 + ( mesh.mHasNormals ? 1 : 0 ) + ( mesh.mHasTexCoords ? 1 : 0 );
	minfo.vertexBuffers.resize( buffersUsed );
	glGenBuffers( buffersUsed, &minfo.vertexBuffers[0] );
	//Index data will be stored in the last buffer

	glBindVertexArray( minfo.vao );
	//Index Data
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, minfo.vertexBuffers[ buffersUsed - 1 ] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, mesh.mFaces.size() * sizeof( uint_ ) * 3, &mesh.mFaces[0] , GL_STATIC_DRAW );

	byte_t nextBuff = 0;
	//Vertex Data
	glBindBuffer( GL_ARRAY_BUFFER, minfo.vertexBuffers[nextBuff] );
	glBufferData( GL_ARRAY_BUFFER, mesh.mVerts.size() * sizeof( float ) * 3, &mesh.mVerts[0], GL_STATIC_DRAW );
	glVertexAttribPointer( BE_SB_POSITION, 3 , GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( BE_SB_POSITION );
	nextBuff++;

	if( mesh.mHasNormals ) {
		//Normal Data
		glBindBuffer( GL_ARRAY_BUFFER, minfo.vertexBuffers[nextBuff] );
		glBufferData( GL_ARRAY_BUFFER, mesh.mNormals.size() * sizeof( float ), &mesh.mNormals[0], GL_STATIC_DRAW );
		glVertexAttribPointer( BE_SB_NORMAL, 3 , GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( BE_SB_NORMAL );
		nextBuff++;
	}

	if( mesh.mHasTexCoords ) {
		//Texture Data
		glBindBuffer( GL_ARRAY_BUFFER, minfo.vertexBuffers[nextBuff] );
		glBufferData( GL_ARRAY_BUFFER, mesh.mTexCoords.size() * sizeof( float ), &mesh.mTexCoords[0], GL_STATIC_DRAW );
		glVertexAttribPointer( BE_SB_TEX1, 2 , GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( BE_SB_TEX1 );
		nextBuff++;
	}

	glBindVertexArray( 0 );
}

void BOpenGLRenderer::renderMesh( BMesh::RID mid, const BNode3D &node )
{
	//Mesh data is invalid
	if ( !mMeshData.count( mid ) ) {
		BLogger::error( " [Renderer]Mesh Data not loaded" );
		return;
	}
	const MeshInfo &minfo = mMeshData[mid];

	//TODO:[Performace] Stop using matrix stacks. Or at least use GLM matrices
	mModelViewMatrix.PushMatrix();
	mModelViewMatrix.Translate( node.getAbsPosition().x, node.getAbsPosition().y, node.getAbsPosition().z );
	//mModelViewMatrix.MultMatrix( glm::value_ptr(glm::toMat4( node.getAbsRotation() )) );

	setMaterial( mActiveMaterial );

	glBindVertexArray( minfo.vao );
	if ( minfo.hasIndexData ) {
		glDrawElements( minfo.primitiveType , minfo.numVertices, GL_UNSIGNED_INT, 0 );
	} else {
		glDrawArrays( minfo.primitiveType , 0, minfo.numVertices );
	}
	checkGLErrors();

	glBindVertexArray( 0 );
	mModelViewMatrix.PopMatrix();
}

void BOpenGLRenderer::renderParticleSystem( const BParticleSystem &ps )
{
	GLuint buff;
	glGenBuffers( 1, &buff );

	glBindBuffer( GL_ARRAY_BUFFER, buff );
	glBufferData( GL_ARRAY_BUFFER, ps.activeParticles() * sizeof( vec3 ), &( ps.positionData()[0] ), GL_DYNAMIC_DRAW );

	glEnableVertexAttribArray( BMesh::B_MD_POSITION );
	glVertexAttribPointer( BMesh::B_MD_POSITION , 3 , GL_FLOAT, GL_FALSE, 0, 0 );

	glDrawArrays( GL_POINTS, 0, ps.activeParticles() );

	glDeleteBuffers( 1, &buff );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

template <class LLW>
void BBaseRenderer<LLW>::initShaders()
{
	BShaderDescription *sDesc = new BShaderDescription;
	sDesc->id = GLT_SHADER_FLAT;
	sDesc->parameters["vColor"] = B_SD_VEC4;
	sDesc->predefParameters.push_back( B_SP_MVPMatrix );

	mShaderDescriptions[ "Basic" ].reset( sDesc );

	sDesc = new BShaderDescription;
	sDesc->id = GLT_SHADER_POINT_LIGHT_DIFF;
	sDesc->parameters["vLightPos"] = B_SD_VEC3;
	sDesc->parameters["vColor"] = B_SD_VEC4;
	sDesc->predefParameters.push_back( B_SP_MVMatrix );
	sDesc->predefParameters.push_back( B_SP_PMatrix );

	mShaderDescriptions[ "BasicWLight" ].reset( sDesc );

	sDesc = new BShaderDescription;
	sDesc->id = GLT_SHADER_TEXTURE_REPLACE;
	sDesc->parameters["textureUnit0"] = B_SD_TEXTUREID;
	sDesc->predefParameters.push_back( B_SP_MVPMatrix );

	mShaderDescriptions[ "BasicTextured" ].reset( sDesc );
}

BMaterial *BOpenGLRenderer::newMaterial( std::string const &shaderName )
{
	auto loc = mShaderDescriptions.find( shaderName );
	if ( loc != mShaderDescriptions.end() ) {
		return BMaterial::create( loc->second );
	} else {
		BLogger::error( "[Renderer] Asked for unknown shader: " + shaderName );
		return NULL;
	}
};

void BOpenGLRenderer::setMaterial( BMaterial *mat )
{
	if ( mat ) {
		if ( mat->isComplete() ) {
			//HACK: This still uses GLT Shader Manager to load/bind/compile shaders.
			boost::shared_ptr<BShaderDescription> shaderDesc( mat->getShaderDescription() );
			GLint shaderID = mShaderManager.GetStockShader( ( GLT_STOCK_SHADER ) shaderDesc->id );

			GLint paramPos;

			glUseProgram( shaderID );

			//checkGLErrors();

			//TODO: [Performance] Only update parameters that have changed.
			//First load any predefined parameters that the shader might need
			for ( auto param = shaderDesc->predefParameters.begin(); param != shaderDesc->predefParameters.end(); ++param ) {
				//TODO: [Performance] Calls to getUniformLocation can be avoided
				//		If I link all the shaders on program init and get and store locations then I'll have the required indices
				switch( *param ) {
						//TODO: Error checking.
					case B_SP_MVPMatrix:
						paramPos = glGetUniformLocation( shaderID , "mvpMatrix" );
						//checkGLErrors();
						glUniformMatrix4fv( paramPos, 1, GL_FALSE, mTransformPipeline.GetModelViewProjectionMatrix() );
						break;
					case B_SP_MVMatrix:
						paramPos = glGetUniformLocation( shaderID , "mvMatrix" );
						glUniformMatrix4fv( paramPos, 1, GL_FALSE, mTransformPipeline.GetModelViewMatrix() );
						break;
					case B_SP_PMatrix:
						paramPos = glGetUniformLocation( shaderID , "pMatrix" );
						glUniformMatrix4fv( paramPos, 1, GL_FALSE, mTransformPipeline.GetProjectionMatrix() );
						break;
						//TODO: [MissingFunctionality] Add the TIME uniform
				}
			}

			//Next get uniform locations for custom parameters and send data;
			for ( auto param = shaderDesc->parameters.begin(); param != shaderDesc->parameters.end(); ++param ) {
				switch( param->second ) {
					case B_SD_VEC2:
						paramPos = glGetUniformLocation( shaderID , param->first.c_str() );
						glUniform2fv( paramPos, 1, ( GLfloat * ) mat->getParam( param->first ) );
						break;
					case B_SD_VEC3:
						paramPos = glGetUniformLocation( shaderID , param->first.c_str() );
						glUniform3fv( paramPos, 1, ( GLfloat * ) mat->getParam( param->first ) );
						break;
					case B_SD_VEC4:
						paramPos = glGetUniformLocation( shaderID , param->first.c_str() );
						glUniform4fv( paramPos, 1, ( GLfloat * ) mat->getParam( param->first ) );
						break;
					case B_SD_FLOAT:
						paramPos = glGetUniformLocation( shaderID , param->first.c_str() );
						glUniform1f( paramPos, *( ( GLfloat * ) mat->getParam( param->first ) ) );
						break;
					case B_SD_MAT4:
						paramPos = glGetUniformLocation( shaderID , param->first.c_str() );
						glUniformMatrix4fv( paramPos, 1, GL_FALSE, ( GLfloat * ) mat->getParam( param->first ) );
						break;
					case B_SD_TEXTUREID:
						paramPos = glGetUniformLocation( shaderID , param->first.c_str() );
						//TODO:[INCOMPLETE] Load the required texture and pass it properly.
						glUniform1i( paramPos, 0 ); //HACK: Uses the texture found in the default texture object
				}
			}
			mActiveMaterial = mat;
		} else {
			BLogger::error( "[Renderer] Material incomplete" );
		}
	} else {
		BLogger::error( "[Renderer] Asked to set to a NULL material" );
	}
}

void BOpenGLRenderer::checkGLErrors()
{
	switch ( GLenum en = glGetError() ) {
		case GL_NO_ERROR:
			return;
		case GL_INVALID_ENUM:
			std::cout << "Invalid Enum" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cout << "Invalid Val" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cout << "Invalid Op" << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "Stack Overflow" << std::endl;
			break;
		default:
			std::cout << "Unknown error: " << en << std::endl;
	}
}