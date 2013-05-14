#include "BMeshGenerator.h"

using namespace Bolts;

intrusive_ptr<BMesh>::t BMeshGenerator::buildCube( float size, bool wNormals )
{
	intrusive_ptr<BMesh>::t mesh( new BMesh( 24 ) , false );

	std::vector<float> vertices( 24 );
	vertices[0] = vertices[1] = vertices[2] = size;
	vertices[3] = vertices[4] = size;
	vertices[5] = -size;
	vertices[6] = -size;
	vertices[7] = size;
	vertices[8] = -size;
	vertices[9] = -size;
	vertices[10] = size;
	vertices[11] = size;
	vertices[12] = size;
	vertices[13] = -size;
	vertices[14] = size;
	vertices[15] = size;
	vertices[16] = -size;
	vertices[17] = -size;
	vertices[18] = -size;
	vertices[19] = -size;
	vertices[20] = -size;
	vertices[21] = -size;
	vertices[22] = -size;
	vertices[23] = size;

	std::vector<uint_> indices( 36 );
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	indices[6] = 6;
	indices[7] = 2;
	indices[8] = 1;
	indices[9] = 6;
	indices[10] = 1;
	indices[11] = 5;

	indices[12] = 3;
	indices[13] = 2;
	indices[14] = 6;
	indices[15] = 3;
	indices[16] = 6;
	indices[17] = 7;

	indices[18] = 0;
	indices[19] = 3;
	indices[20] = 7;
	indices[21] = 0;
	indices[22] = 7;
	indices[23] = 4;

	indices[24] = 1;
	indices[25] = 0;
	indices[26] = 4;
	indices[27] = 1;
	indices[28] = 4;
	indices[29] = 5;

	indices[30] = 4;
	indices[31] = 7;
	indices[32] = 6;
	indices[33] = 4;
	indices[34] = 6;
	indices[35] = 5;

	if ( wNormals ) {
		std::vector<float> normals( 24 );
		normals[0] = normals[1] = normals[2] = 0.57735f;
		normals[3] = normals[4] = 0.57735f;
		normals[5] = -0.57735f;
		normals[6] = -0.57735f;
		normals[7] = 0.57735f;
		normals[8] = -0.57735f;
		normals[9] = -0.57735f;
		normals[10] = 0.57735f;
		normals[11] = 0.57735f;
		normals[12] = 0.57735f;
		normals[13] = -0.57735f;
		normals[14] = 0.57735f;
		normals[15] = 0.57735f;
		normals[16] = -0.57735f;
		normals[17] = -0.57735f;
		normals[18] = -0.57735f;
		normals[19] = -0.57735f;
		normals[20] = -0.57735f;
		normals[21] = -0.57735f;
		normals[22] = -0.57735f;
		normals[23] = 0.57735f;
		mesh->loadDataForBuffer( normals, BMesh::B_MD_NORMAL );
	}

	std::vector<float> tex0( 16 );
	for ( int i = 0; i < 2; ++i ) {
		tex0[i * 8 + 0] = tex0[i * 8 + 1] = 0.0f;
		tex0[i * 8 + 2] = 0.0f;
		tex0[i * 8 + 3] = 1.0f;
		tex0[i * 8 + 4] = 1.0f;
		tex0[i * 8 + 5] = 1.0f;
		tex0[i * 8 + 6] = 1.0f;
		tex0[i * 8 + 7] = 0.0f;
	}
	mesh->loadDataForBuffer( tex0, BMesh::B_MD_TEX0 );

	mesh->loadDataForBuffer( vertices, BMesh::B_MD_POSITION );
	mesh->loadIndexData( indices );
	return mesh;
}

intrusive_ptr<BMesh>::t BMeshGenerator::buildGrid( float square_size, uint_ size )
{
	uint_ verts = ( size + size + 2 ) * 3;

	intrusive_ptr<BMesh>::t mesh( new BMesh( verts ) , false );
	std::vector<float> vertices;
	vertices.reserve( verts );
	//std::vector< uint_ > indices;
	//indices.reserve(3 * size * size);
	float startx = square_size * ( float )size / 2;
	float startz = startx;
	float nx = startx, nz = startz;
	for ( uint_ i = 0; i <= size; ++i ) {
		vertices.push_back( nx );
		vertices.push_back( 0.0f );
		vertices.push_back( startz );
		vertices.push_back( nx );
		vertices.push_back( 0.0f );
		vertices.push_back( -startz );

		vertices.push_back( startx );
		vertices.push_back( 0.0f );
		vertices.push_back( nz );
		vertices.push_back( -startx );
		vertices.push_back( 0.0f );
		vertices.push_back( nz );

		nx -= square_size;
		nz -= square_size;
	}
	mesh->setPrimitiveType( BMesh::B_MP_LINES );
	mesh->loadDataForBuffer( vertices, BMesh::B_MD_POSITION );
	//mesh->loadIndexData(indices);

	return mesh;
}

intrusive_ptr<BMesh>::t BMeshGenerator::buildArrow( char dir, float length, float width )
{
	uint_ vert_floats = 6 * 3;//6 Verts

	intrusive_ptr<BMesh>::t mesh( new BMesh( vert_floats ) , false );
	std::vector<float> vertices;
	vertices.reserve( vert_floats );

	switch( dir ) {
		case 'x':
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( length );
			vertices.push_back( 0. );
			vertices.push_back( 0. );

			vertices.push_back( length );
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( length );
			vertices.push_back( width );
			vertices.push_back( 0. );

			vertices.push_back( length );
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( length );
			vertices.push_back( -width );
			vertices.push_back( 0. );
			break;
		case 'y':
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( length );
			vertices.push_back( 0. );

			vertices.push_back( 0. );
			vertices.push_back( length );
			vertices.push_back( 0. );
			vertices.push_back( width );
			vertices.push_back( length );
			vertices.push_back( 0. );

			vertices.push_back( 0. );
			vertices.push_back( length );
			vertices.push_back( 0. );
			vertices.push_back( -width );
			vertices.push_back( length );
			vertices.push_back( 0. );
			break;
		case 'z':
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( length );

			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( length );
			vertices.push_back( width );
			vertices.push_back( 0. );
			vertices.push_back( length );

			vertices.push_back( 0. );
			vertices.push_back( 0. );
			vertices.push_back( length );
			vertices.push_back( -width );
			vertices.push_back( 0. );
			vertices.push_back( length );
			break;
		default:
			BLogger::error( "[MeshGenerator] Error generating arrow mesh. Expected dir 'x','y','z'." );
	}
	mesh->setPrimitiveType( BMesh::B_MP_LINES );
	mesh->loadDataForBuffer( vertices, BMesh::B_MD_POSITION );

	return mesh;
}