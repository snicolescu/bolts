#include "Rendering.h"
#include "Core/logger.h"
#include "Core/helpers.h"

#ifdef BOLTS_OPENGL

#include "Rendering/GL/GLDriver.h"
#include "Rendering/GL/GLSLProgram.h"
#include "Rendering/GL/GLVertexBuffer.h"

typedef Bolts::Rendering::GLDriver APIDriver;
typedef Bolts::Rendering::GLDriverStn APIDriverStn;

#else
#error The only available renderer is GLRenderer. define BOLTS_OPENGL in order to use the OpenGL renderer.
#endif

using namespace Bolts::Rendering;

Bolts::Rendering::Renderer::Renderer() : m_activeProgram( NULL ), m_buffersDirty( false )
{

}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	APIDriverStn::Instance().Init ();

	return true;
}

void Renderer::EnableVertexBuffer( VertexBufferCPtr newBuffer )
{
	m_buffersDirty = true;
	m_enabledBuffers.insert( newBuffer );
}


void Renderer::ResetVertexBuffers()
{
	//TODO: Tell driver vertex indices are no longer used !
	m_buffersDirty = true;
	std::for_each( m_enabledBuffers.cbegin(), m_enabledBuffers.cend(), [&] ( const VertexBufferCPtr & vbuffer ) {
		APIDriverStn::Instance().DisableVertexBuffer ( *vbuffer );
	} );
	m_enabledBuffers.clear ();
}

void Bolts::Rendering::Renderer::EnableProgram( GPUProgramPtr GPUProg )
{
	if ( GPUProg != m_activeProgram ) {
		if ( !APIDriverStn::Instance().EnableGPUProgram( GPUProg ) ) {
			//TODO LOG: Error enabling GPUProgram
			//TODO: Set to a pink bad shader
			return;
		}
		m_buffersDirty = true;
		m_activeProgram = GPUProg;
	}
}

void Bolts::Rendering::Renderer::Draw( PrimitiveType prim )
{
	if ( !m_activeProgram || m_enabledBuffers.empty () ) {
		//TODO LOG: Attempting to draw without attribute buffers bound
		return;
	}
	MatchVertexBuffers();

	uint32_t numVertices = ( *m_enabledBuffers.begin () )->GetNumVertices();
	APIDriverStn::Instance().Draw( prim, numVertices );
}

void Bolts::Rendering::Renderer::Clear()
{
	APIDriverStn::Instance ().Clear();
}

void Bolts::Rendering::Renderer::MatchVertexBuffers()
{
	if ( !m_activeProgram || m_enabledBuffers.empty () || !m_buffersDirty ) {
		return;
	}

	std::for_each ( m_enabledBuffers.cbegin (), m_enabledBuffers.cend (), [] ( VertexBufferCPtr vertexBuffer ) {
		APIDriverStn::Instance ().MatchVertexBuffer ( *vertexBuffer.get() );
	} );
}
