#include "BOGLWrapper.h"

using namespace Bolts;

void BOGLWrapper::init( int w, int h )
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

void BOGLWrapper::viewportChanged( int width, int height )
{
	glViewport( 0, 0, width, height );
}

BOGLWrapper::MeshInfo *BOGLWrapper::loadMesh( const BMesh &mesh )
{
	//TODO: Error checking (see if buffers were actually created, etc.
	MeshInfo *minfo = new MeshInfo;

	minfo->hasIndexData = mesh.hasIndexData();
	BMesh::DataInfo mDataInfo = mesh.getDataInfo();

	switch( mesh.getPrimitiveType() ) {
		case BMesh::B_MP_LINES:
			minfo->primitiveType = GL_LINES;
			break;
		case BMesh::B_MP_TRIANGLES:
			minfo->primitiveType = GL_TRIANGLES;
			break;
		case BMesh::B_MP_TRIANGLESTRIP:
			minfo->primitiveType = GL_TRIANGLE_STRIP;
			break;
	}

	if ( minfo->hasIndexData ) {
		minfo->numVertices = mesh.getNumIndices();
	} else {
		minfo->numVertices = mesh.getNumVertices();
	}

	glGenVertexArrays( 1, &minfo->vao );

	minfo->vertexBuffers.resize( mDataInfo.buffersUsed );
	byte_t nextBuff = 0;
	glGenBuffers( mDataInfo.buffersUsed, &minfo->vertexBuffers[0] );
	//Index data will be stored in the last buffer

	glBindVertexArray( minfo->vao );

	if ( mesh.hasIndexData() ) {
		const std::vector< uint_ > &indexData = mesh.getIndexData();
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, minfo->vertexBuffers[ mDataInfo.buffersUsed - 1 ] );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof( uint_ ), &indexData[0] , GL_STATIC_DRAW );
	}

	for ( int i = 0; i < mDataInfo.hasData.size() ; ++i ) {
		if ( mDataInfo.hasData[i] ) {
			const std::vector< float > &verData = mesh.getData( ( BMesh::B_MESH_DATA ) i );
			glBindBuffer( GL_ARRAY_BUFFER, minfo->vertexBuffers[nextBuff] );
			glBufferData( GL_ARRAY_BUFFER, verData.size() * sizeof( float ), &verData[0], GL_STATIC_DRAW );
			glVertexAttribPointer( i, Shaders::ShaderDataSize[i], GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray( i );
			nextBuff++;
		}
	}

	glBindVertexArray( 0 );
	return minfo;
}

BOGLWrapper::MeshInfo *BOGLWrapper::loadMesh( const BObjMesh &mesh )
{
	//TODO: Error checking (see if buffers were actually created, etc.
	MeshInfo *minfo = new MeshInfo;

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
	glVertexAttribPointer( Shaders::ShaderDataBinding::B_SB_POSITION, 3 , GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( Shaders::ShaderDataBinding::B_SB_POSITION );
	nextBuff++;

	if( mesh.mHasNormals ) {
		//Normal Data
		glBindBuffer( GL_ARRAY_BUFFER, minfo.vertexBuffers[nextBuff] );
		glBufferData( GL_ARRAY_BUFFER, mesh.mNormals.size() * sizeof( float ), &mesh.mNormals[0], GL_STATIC_DRAW );
		glVertexAttribPointer( Shaders::ShaderDataBinding::B_SB_NORMAL, 3 , GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( Shaders::ShaderDataBinding::B_SB_NORMAL );
		nextBuff++;
	}

	if( mesh.mHasTexCoords ) {
		//Texture Data
		glBindBuffer( GL_ARRAY_BUFFER, minfo.vertexBuffers[nextBuff] );
		glBufferData( GL_ARRAY_BUFFER, mesh.mTexCoords.size() * sizeof( float ), &mesh.mTexCoords[0], GL_STATIC_DRAW );
		glVertexAttribPointer( Shaders::ShaderDataBinding::B_SB_TEX0, 2 , GL_FLOAT, GL_FALSE, 0, 0 );
		glEnableVertexAttribArray( Shaders::ShaderDataBinding::B_SB_TEX0 );
		nextBuff++;
	}

	glBindVertexArray( 0 );
	return minfo;
}