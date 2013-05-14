#ifndef Rendering_h__
#define Rendering_h__

#include "Core\types.h"
#include "Rendering/Common/RenderingCommon.h"
#include "Rendering/VertexBufferFwd.h"
#include "Rendering/GPUProgramFwd.h"
#include <set>

namespace Bolts {
	namespace Rendering {

		class Renderer {
			public:
				Renderer();;
				~Renderer();

				bool	Initialize();

				void	EnableVertexBuffer( VertexBufferCPtr);
				void	ResetVertexBuffers();

				void	EnableProgram( GPUProgramPtr);

				void	Draw( PrimitiveType prim);

				void	Clear();
			private:
				typedef std::set< VertexBufferCPtr> bufferContainer_t;

				void	MatchVertexBuffers();

				bufferContainer_t	m_enabledBuffers;
				//True if the vertex buffers haven't been matched to the shader yet.
				bool				m_buffersDirty;
				GPUProgramPtr		m_activeProgram;
		};

	};

};
#endif // Rendering_h__

