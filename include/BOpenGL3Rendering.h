#pragma once
#include <gl/glew.h>
#include <gl/GL.h>

#include "BGlobals.h"
#include "BShader.h"
#include "BMesh.h"
#include "BObjMesh.h"
#include "BGLSLShader.h"

namespace Bolts {
	namespace Rendering {

		class OGL3Wrapper {
			public:
				typedef GLSLShader ShaderClass;
				//TODO: Move this class into it's own file and make it more robust

				void init( int width, int height );
				void viewportChanged( int width, int height );

				MeshInfo *loadMesh( const BMesh &mesh );
				MeshInfo *loadMesh( const BObjMesh &mesh );

			private:

		};

		struct MeshInfo: public ref_counted_type {
			MeshInfo(): vao( 0 ), primitiveType( GL_TRIANGLES ) {};

			std::vector< GLuint > vertexBuffers;
			GLuint vao;
			size_t numVertices;
			bool hasIndexData;
			GLuint primitiveType;

			~MeshInfo() {
				if ( vertexBuffers.size() > 0 ) {
					glDeleteBuffers( vertexBuffers.size(), &vertexBuffers[0] );
				}
				if ( vao != 0 ) {
					glDeleteVertexArrays( 1, &vao );
				}
			}
		};
	};
};