#include "Core\logger.h"

#include "Rendering/GL/GLVertexBuffer.h"
#include "Rendering/GL/GLDriver.h"
#include "Rendering/Common/GPUVertexBufferBase.h"

using namespace Bolts;

Rendering::GLVertexBuffer::GLVertexBuffer(): Rendering::GPUVertexBufferBase()
{
	m_bufferID = GLDriverStn::Instance().GenerateBufferID ();
}

Rendering::GLVertexBuffer::GLVertexBuffer( const Rendering::GPUBufferDescription &desc ): Rendering::GPUVertexBufferBase( desc )
{
	m_bufferID = GLDriverStn::Instance().GenerateBufferID ();
}

bool Rendering::GLVertexBuffer::HasID() const
{
	return ( m_bufferID != 0 );
}

void Rendering::GLVertexBuffer::FreeID()
{
	GLDriverStn::Instance().FreeBufferID ( m_bufferID );
	m_bufferID = 0;
}

Rendering::GLVertexBuffer::~GLVertexBuffer()
{
	FreeID();
}

void Bolts::Rendering::GLVertexBuffer::UploadData() const
{
	if ( !HasID () ) {
		//TODO LOG: GLVertexBuffer has no ID

	}
	if ( m_isDirty ) {
		glBindBuffer( GL_ARRAY_BUFFER, m_bufferID );

		glBufferData( GL_ARRAY_BUFFER, m_bufferSize, m_bufferData, GLDriverStn::Instance ().GetVertexUsageHint( m_usage ) );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		m_isDirty = false;
	}
}
