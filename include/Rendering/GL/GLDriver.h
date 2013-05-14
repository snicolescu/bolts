#ifndef GLDriver_h__
#define GLDriver_h__

#include <vector>
#include <array>

#include "glew/GL/glew.h"
#include "loki/Singleton.h"
#include "Rendering/Common/GPUVertexBufferBase.h"
#include "Rendering/Common/RenderingCommon.h"
#include "Rendering/GL/GLSLProgramFwd.hpp"

namespace Bolts {

	namespace Rendering {

		class GLVertexBuffer;

		class GLDriver{
		public:
			GLDriver():m_Initialized(false) {}
	
			//TODO: Return success or failure
			void	Init();
			void	SetFlag();

			void	MatchVertexBuffer( const GLVertexBuffer&);
			void	DisableVertexBuffer( const GLVertexBuffer&);
			void	SetDefaultAttributePositions( GLuint programID);
			bool	EnableGPUProgram( GLSLProgramPtr);

			GLuint	GenerateBufferID();
			void	FreeBufferID( GLuint );
			GLenum	GetVertexUsageHint( BufferUsageHint m_usage );
			GLenum	GetPrimitiveType( PrimitiveType m_prim );
			GLenum	GetVarType( GPUVarType m_generalType) { return g_GPUNativeVarType[m_generalType]; }


			void	Draw( PrimitiveType, size_t);
			//TODO: Add clear flags
			void	Clear();
		private:
			static GLenum		g_GPUNativeVarType[ _BGVT_LAST];

			bool				m_Initialized;
		};

		typedef class Loki::SingletonHolder< 
			GLDriver,
			Loki::CreateUsingNew,
			Loki::DefaultLifetime
		> GLDriverStn;

	};

};
#endif // GLDriver_h__
