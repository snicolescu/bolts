#include "Helpers/MeshBuilder.h"
#include "Rendering/VertexBuffer.hpp"

using namespace Bolts;

Rendering::VertexBufferPtr Bolts::MeshBuilder::GenCubeMesh( float size)//, bool withUV, bool withNormals)
{
	size;
	Rendering::GPUBufferDescription desc;
	desc.AddStream ( Rendering::GPUBufferDescription::StreamDesc( Rendering::BGSS_POSITION, Rendering::BGVT_FLOAT, 3, 0) );
	desc.AddStream ( Rendering::GPUBufferDescription::StreamDesc( Rendering::BGSS_NORMALS, Rendering::BGVT_FLOAT, 3, 3*sizeof(float)) );

	Rendering::VertexBufferPtr vbuffer = new Rendering::VertexBuffer( desc);
	vbuffer->SetNumberOfVertices ( 2*6);

	float cubeVerts[ 12 * 6] = {
		//Right
		1.f, 1.f, 1.f, 1.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 1.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 1.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 1.f, 0.f, 0.f,
		1.f, -1.f, -1.f, 1.f, 0.f, 0.f,
		1.f, 1.f, -1.f, 1.f, 0.f, 0.f,
		//Front
		1.f, 1.f, -1.f, 0.f, 0.f, -1.f,
		1.f, -1.f, -1.f, 0.f, 0.f, -1.f,
		-1.f, 1.f, 1.f, 0.f, 0.f, -1.f,
		-1.f, 1.f, 1.f, 0.f, 0.f, -1.f,
		1.f, -1.f, 1.f, 0.f, 0.f, -1.f,
		1.f, 1.f, -1.f, 0.f, 0.f, -1.f,
		
	};

	vbuffer->SetData ( Rendering::BUH_STATIC_DRAW, cubeVerts, sizeof(cubeVerts));
	return vbuffer;
}
