#include <Rendering/Common/GPUVertexBufferBase.h>
#include "Core/helpers.h"
#include <cstring>

using namespace Bolts;


Bolts::Rendering::GPUVertexBufferBase::GPUVertexBufferBase() : m_bufferData( NULL ), m_bufferSize( 0 ), m_isDirty( false ), m_description(), m_usage( BUH_STATIC_DRAW )
{

}

Bolts::Rendering::GPUVertexBufferBase::GPUVertexBufferBase( const GPUBufferDescription &desc ) : m_bufferData( NULL ), m_bufferSize( 0 ), m_isDirty( false ), m_description( desc )
{

}

Rendering::GPUVertexBufferBase::~GPUVertexBufferBase()
{
	SAFE_FREE( m_bufferData );
}

bool Rendering::GPUVertexBufferBase::SetData( BufferUsageHint usage, void *source, size_t dataSize, bool makeCopy /*= true*/ )
{
	if ( makeCopy ) {
		//TODO USABILITY: Memory Managed alloc, easier to keep track of data
		//TODO PERF: Memory Pool
		if ( m_bufferSize != dataSize ) {
			SAFE_FREE( m_bufferData )
			m_bufferData = malloc( dataSize );

			if ( m_bufferData == NULL ) {
				//TODO LOG: Could not allocate data for GPU Buffer
				return false;
			}
		}
		memcpy( m_bufferData, source, dataSize );
	} else {
		SAFE_FREE( m_bufferData )
		m_bufferData = source;
	}

	m_usage = usage;
	m_bufferSize = dataSize;
	m_isDirty = true;

	return true;
}

void Bolts::Rendering::GPUVertexBufferBase::ClearData()
{
	SAFE_FREE( m_bufferData );
	m_bufferSize = 0;
	m_isDirty = false;
}


void Rendering::GPUBufferDescription::AddStream( const StreamDesc &newStream )
{
	BOOST_ASSERT_MSG( m_streamMap.count( newStream.semantic ) == 0, "[W][4]GPU Buffer description already has a description for this semantic" );
	m_streamMap[ newStream.semantic ] = newStream;
	m_strideDirty = true;
}

void Rendering::GPUBufferDescription::ReplaceStream( const StreamDesc &newStream )
{
	m_streamMap[ newStream.semantic ] = newStream;
	m_strideDirty = true;
}

uint8_t Rendering::GPUBufferDescription::GetStride() const
{
	if ( !IsBlank() && m_strideDirty ) {
		RecomputeStride();
	}
	return m_stride;
}

void Rendering::GPUBufferDescription::RecomputeStride() const
{
	m_stride = 0;
	std::for_each( m_streamMap.begin(), m_streamMap.end(),
	[&] ( const std::pair< GPUStreamSemantic, GPUBufferDescription::StreamDesc> &stream ) {
		m_stride += stream.second.componentNum * GPUVarSize[stream.second.attributeType];
	} );

	m_strideDirty = false;
}

const Rendering::GPUBufferDescription::stream_map_t &Rendering::GPUBufferDescription::GetStreams() const
{
	return m_streamMap;
}
