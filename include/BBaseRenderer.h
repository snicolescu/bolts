#pragma once
#include "BGlobals.h"
#include "B3DTransform.h"
#include "BMaterial.h"
#include "BShader.h"
#include <vector>

//HACK: Get rid of these dependencies
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

namespace Bolts {

	template <class LowLevelWrapper>
	class BBaseRenderer {
		public:

			void init( int width, int height );
			void viewportChanged( int width, int height );

			void render();

			void loadMesh( RIDType mid, const BMesh &mesh );
			void loadMesh( RIDType mid, const BObjMesh &mesh );
			void unloadMesh( RIDType mid ) {
				mMeshData.erase( mid );
			};
			bool meshLoaded( RIDType mid ) {
				return ( mMeshData.count( mid ) == 1 );
			};

		private:
			struct StaticMesh {
				B3DTransform mTransform;
				RIDType mMeshRID;
				BMaterial *mMaterial;
			};
			typedef typename LowLevelWrapper::MeshInfo MeshInfo;
			typedef typename LowLevelWrapper::ShaderClass Shader;

			GLShaderManager		mShaderManager;			// Shader Manager
			GLMatrixStack		mModelViewMatrix;		// Modelview Matrix
			GLMatrixStack		mProjectionMatrix;		// Projection Matrix
			GLFrustum			mViewFrustum;			// View Frustum
			GLGeometryTransform	mTransformPipeline;		// Geometry Transform Pipeline
			mat4				mViewMatrix;

			LowLevelWrapper mLLW;

			std::vector< QueuedStaticMesh > mStaticMeshes;
			std::map< std::string, boost::shared_ptr<BShaderDescription> > mShaderDescriptions;

			//These maps contain all the data that is defined and used by the LLW
			std::map< RIDType, MeshInfo > mMeshData;
			std::map< ShaderID, Shader > mShaderData;

			void initShaders();
	};
};