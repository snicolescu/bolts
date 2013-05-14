#include "Rendering\GL\GLDriver.h"
#include "Rendering\GL\GLSLProgram.h"
#include "Rendering\GL\GLVertexBuffer.h"

#include "glew/GL/glew.h"

using namespace Bolts;
using namespace Rendering;

GLenum Bolts::Rendering::GLDriver::g_GPUNativeVarType[_BGVT_LAST] = { 
				GL_FLOAT, GL_HALF_FLOAT, 
				GL_BYTE, GL_UNSIGNED_BYTE, 
				GL_SHORT, GL_UNSIGNED_SHORT,
				GL_INT, GL_UNSIGNED_INT
			};

static std::array< std::pair< GLuint,const GLchar*>, 3>	g_GPUBuiltinAttributeNames = {
	std::make_pair( (GLuint) BGSS_POSITION, "a_posL"),
	std::make_pair( (GLuint) BGSS_COLOR0, "a_color0"),
	std::make_pair( (GLuint) BGSS_COLOR0, "a_color")
};

void GLDriver::SetFlag()
{

}

void GLDriver::Init()
{
	if (m_Initialized){
		return;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		//TODO: Log init failure properly
		/* Problem: glewInit failed, something is seriously wrong. */
		printf("GLEW Error: %s\n", glewGetErrorString(err));
	}

	glEnable ( GL_CULL_FACE);
	//glDisable( GL_CULL_FACE);

	glClearColor ( 0.0, 0.0, 0.0, 1.0);

	m_Initialized = true;
}

GLuint GLDriver::GenerateBufferID()
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	return bufferID;
}

void GLDriver::FreeBufferID(GLuint bufferID)
{
	glDeleteBuffers(1, &bufferID);
}

GLenum GLDriver::GetVertexUsageHint( BufferUsageHint m_usage )
{
	switch( m_usage){
	case BUH_STATIC_DRAW:
		return GL_STATIC_DRAW;
		break;
	case BUH_DYNAMIC_DRAW:
		return GL_DYNAMIC_DRAW;
		break;
	case BUH_STREAM_DRAW:
		return GL_STREAM_DRAW;
		break;
	case BUH_STATIC_READ:
		return GL_STATIC_READ;
		break;
	case BUH_DYNAMIC_READ:
		return GL_DYNAMIC_READ;
		break;
	case BUH_STREAM_READ:
		return GL_STREAM_READ;
		break;
	case BUH_STATIC_COPY:
		return GL_STATIC_COPY;
		break;
	case BUH_DYNAMIC_COPY:
		return GL_DYNAMIC_COPY;
		break;
	case BUH_STREAM_COPY:
		return GL_STREAM_COPY;
		break;
	default:
		//TODO LOG: Unknown BUH
		return GL_STATIC_DRAW;
	}
}

bool Bolts::Rendering::GLDriver::EnableGPUProgram( GLSLProgramPtr GPUProg)
{
	GPUProg->Enable ();

	if ( !GPUProg->IsReady ()){
		//TODO: Print failure reason

		return false;
	}

	return true;
}

void Bolts::Rendering::GLDriver::Draw( PrimitiveType primitive, size_t count)
{
	glDrawArrays( GetPrimitiveType( primitive), 0, count);
}

GLenum Bolts::Rendering::GLDriver::GetPrimitiveType( PrimitiveType m_prim )
{
	switch( m_prim){
	case BPT_TRIANGLES:
		return GL_TRIANGLES;
	case BPT_TRIANGLESTRIP:
		return GL_TRIANGLE_STRIP;
	default:
		//TODO LOG: Unsupported primitive type: m_prim
		return GL_TRIANGLES;
	}
}

void Bolts::Rendering::GLDriver::Clear()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void Bolts::Rendering::GLDriver::MatchVertexBuffer( const GLVertexBuffer& vbuffer)
{
	vbuffer.UploadData ();
	glBindBuffer( GL_ARRAY_BUFFER, vbuffer.m_bufferID);

	auto &VBufferDesc ( vbuffer.GetDescription() );
	
	std::for_each( VBufferDesc.GetStreams().cbegin(), VBufferDesc.GetStreams().cend(), [&] (const GPUBufferDescription::stream_map_t::value_type& stream){
		const auto& streamDescription( stream.second);
		glEnableVertexAttribArray( streamDescription.semantic);
		//TODO: Select proper function from attribute type
		glVertexAttribPointer( streamDescription.semantic, 
				streamDescription.componentNum, 
				GetVarType (streamDescription.attributeType), 
				GL_TRUE, 
				VBufferDesc.GetStride(), 
				(const GLvoid*) streamDescription.offset);
	});

	glBindBuffer( GL_ARRAY_BUFFER, NULL);
	//VBufferDesc.GetFirstStream()->second.
}

void Bolts::Rendering::GLDriver::DisableVertexBuffer( const GLVertexBuffer& vbuffer)
{
	auto &VBufferDesc ( vbuffer.GetDescription() );

	std::for_each( VBufferDesc.GetStreams().cbegin(), VBufferDesc.GetStreams().cend(), [&] (const GPUBufferDescription::stream_map_t::value_type& stream){
		const auto& streamDescription( stream.second);
		glDisableVertexAttribArray( streamDescription.semantic);
	});
}

void Bolts::Rendering::GLDriver::SetDefaultAttributePositions( GLuint programID)
{
	std::for_each ( g_GPUBuiltinAttributeNames.cbegin (), g_GPUBuiltinAttributeNames.cend(), 
		[&programID] ( std::pair<GLuint, const GLchar*> valuePair){
			glBindAttribLocation( programID, valuePair.first, valuePair.second);
		});
}

