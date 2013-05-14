#pragma once
#include <gl/glew.h>
#include <gl/GL.h>

//#include <SOIL.h>//TODO: DELETE THIS

#include <map>
#include <boost/shared_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

//HACK: Get rid of these dependencies
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

#include "BGlobals.h"
#include "BLogger.h"
#include "BMesh.h"
#include "BObjMesh.h"
#include "BParticleSystem.h"
#include "BShader.h"
#include "BMaterial.h"
#include "BNode.h"

namespace Bolts {
	using namespace Shaders;

	enum BE_SHADER_DATA_BINDING {BE_SB_POSITION = 0, BE_SB_COLOR, BE_SB_NORMAL, BE_SB_TEX1, BE_SB_LAST};
	const byte_t BE_SHADER_DATA_SIZE[BE_SB_LAST] = { 3, 3, 3, 2};
	//HACK: Mesh Support is really hacked together very poorly.
	//		Use proper conventions for vertex buffers
	class BOpenGLRenderer {
		public:
			BOpenGLRenderer(): mActiveMaterial( NULL ) {};

			void init( int width, int height );
			void viewportChanged( int width, int height );

			void beforeRender();
			void afterRender();

			void setViewMatrix( mat4 vm ) {
				mViewMatrix = vm;
			};

			//Sends mesh data to the GPU
			void loadMesh( RIDType mid, const BMesh &mesh );
			void loadMesh( RIDType mid, const BObjMesh &mesh );
			void unloadMesh( RIDType mid ) {
				mMeshData.erase( mid );
			};
			bool meshLoaded( RIDType mid ) {
				return ( mMeshData.count( mid ) == 1 );
			};

			void renderMesh( RIDType mid, const BNode3D &node );
			void renderParticleSystem( const BParticleSystem &ps );

			BMaterial *newMaterial( std::string const &shaderName );
			void setMaterial( BMaterial *mat );

		private:
			//TODO: Rethink the resource metadata system.
			struct MeshInfo: public ref_counted_type {
				MeshInfo(): vao( 0 ), primitiveType( GL_TRIANGLES ) {};

				std::vector< GLuint > vertexBuffers;
				GLuint vao;
				size_t numVertices;
				bool hasIndexData;
				GLuint primitiveType;

				~MeshInfo() {
					if ( vertexBuffers.size() ) {
						glDeleteBuffers( vertexBuffers.size(), &vertexBuffers[0] );
						glDeleteVertexArrays( 1, &vao );
					}
				}
			};
			typedef intrusive_ptr<MeshInfo>::t MIPtr;

			GLShaderManager		mShaderManager;			// Shader Manager
			GLMatrixStack		mModelViewMatrix;		// Modelview Matrix
			GLMatrixStack		mProjectionMatrix;		// Projection Matrix
			GLFrustum			mViewFrustum;			// View Frustum
			GLGeometryTransform	mTransformPipeline;		// Geometry Transform Pipeline
			mat4				mViewMatrix;

			std::map< RIDType, MeshInfo > mMeshData;

			std::map< std::string, boost::shared_ptr<BShaderDescription> > mShaderDescriptions;
			BMaterial *mActiveMaterial;

			void initShaders();

			void checkGLErrors();
	};
};