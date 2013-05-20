#include "Helpers/MeshBuilder.h"
#include "Rendering/VertexBuffer.hpp"

using namespace Bolts;

Rendering::VertexBufferPtr Bolts::MeshBuilder::GenCubeMesh( float size ) //, bool withUV, bool withNormals)
{
	size;
	Rendering::GPUBufferDescription desc;
	desc.AddStream ( Rendering::GPUBufferDescription::StreamDesc( Rendering::BGSS_POSITION, Rendering::BGVT_FLOAT, 3, 0 ) );
	desc.AddStream ( Rendering::GPUBufferDescription::StreamDesc( Rendering::BGSS_NORMALS, Rendering::BGVT_FLOAT, 3, 3 * sizeof( float ) ) );

	Rendering::VertexBufferPtr vbuffer = new Rendering::VertexBuffer( desc );
	vbuffer->SetNumberOfVertices ( 6 * 6 );

	float cubeVerts[ 6 * 6 * 6] = {
		//Right +X
		1.f, 1.f, 1.f, 1.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 1.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 1.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 1.f, 0.f, 0.f,
		1.f, -1.f, -1.f, 1.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 1.f, 0.f, 0.f,
		//Front +Z
		1.f, 1.f, 1.f, 0.f, 0.f, 1.f,
		-1.f, 1.f, 1.f, 0.f, 0.f, 1.f,
		1.f, -1.f, 1.f, 0.f, 0.f, 1.f,
		-1.f, 1.f, 1.f, 0.f, 0.f, 1.f,
		-1.f, -1.f, 1.f, 0.f, 0.f, 1.f,
		1.f, -1.f, 1.f, 0.f, 0.f, 1.f,
		//Top +Y
		1.f, 1.f, 1.f, 0.f, 1.f, 0.f,
		1.f, 1.f, -1.f, 0.f, 1.f, 0.f,
		-1.f, 1.f, 1.f, 0.f, 1.f, 0.f,
		-1.f, 1.f, -1.f, 0.f, 1.f, 0.f,
		-1.f, 1.f, 1.f, 0.f, 1.f, 0.f,
		1.f, 1.f, -1.f, 0.f, 1.f, 0.f,
		//Left -X
		-1.f, 1.f, 1.f, -1.f, 0.f, 0.f,
		-1.f, 1.f, -1.f, -1.f, 0.f, 0.f,
		-1.f, -1.f, 1.f, -1.f, 0.f, 0.f,
		-1.f, -1.f, -1.f, -1.f, 0.f, 0.f,
		-1.f, -1.f, 1.f, -1.f, 0.f, 0.f,
		-1.f, 1.f, -1.f, -1.f, 0.f, 0.f,
		//Back -Z
		1.f, -1.f, -1.f, 0.f, 0.f, -1.f,
		-1.f, 1.f, -1.f, 0.f, 0.f, -1.f,
		1.f, 1.f, -1.f, 0.f, 0.f, -1.f,
		-1.f, -1.f, -1.f, 0.f, 0.f, -1.f,
		-1.f, 1.f, -1.f, 0.f, 0.f, -1.f,
		1.f, -1.f, -1.f, 0.f, 0.f, -1.f,
		//Bottom -Y
		1.f, -1.f, 1.f, 0.f, -1.f, 0.f,
		-1.f, -1.f, 1.f, 0.f, -1.f, 0.f,
		1.f, -1.f, -1.f, 0.f, -1.f, 0.f,
		-1.f, -1.f, 1.f, 0.f, -1.f, 0.f,
		-1.f, -1.f, -1.f, 0.f, -1.f, 0.f,
		1.f, -1.f, -1.f, 0.f, -1.f, 0.f
	};

	vbuffer->SetData ( Rendering::BUH_STATIC_DRAW, cubeVerts, sizeof( cubeVerts ) );
	return vbuffer;
}
