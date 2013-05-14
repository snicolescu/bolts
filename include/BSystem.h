#pragma once
#include <set>
#include <algorithm>

#include "BGlobals.h"
#include "BResourceManagers.h"
#include "BMeshGenerator.h"
#include "BCamera.h"
#include "BNode.h"
#include "BConfig.h"

namespace Bolts {
	class BSystem {
		public:
			BSystem(): mMeshManager( mRenderer ), mObjMeshManager( mRenderer ) {};

			void init();
			void initRenderer( int width, int height ) {
				mRenderer.init( width, height );
			};
			void initScene( float_ width, float_ length, float_ height ) {
				mSceneManager.setSceneSize( width, length, height );
			};

			void viewportChanged( int newWidth, int newHeight ) {
				mRenderer.viewportChanged( newWidth, newHeight );
			};

			BRenderer &getRenderer() {
				return mRenderer;
			};
			BMeshManager &getMeshManager()  {
				return mMeshManager;
			};
			BObjMeshManager &getObjMeshManager()  {
				return mObjMeshManager;
			};
			BMeshGenerator &getMeshGenerator() {
				return mMeshGenerator;
			};
			BSceneManager &getSM() {
				return mSceneManager;
			};

			mat4 getViewMatrix() {
				if ( mActiveCamera ) {
					return mActiveCamera->getViewMatrix();
				} else {
					return mat4();
				}
			}

			void setCamera( BCamera *newCam ) {
				mActiveCamera = newCam;
			};

			//NOTE: I may need to change the updating from using DT to using TIME
			//		since it may be different for different objects.
			void update( float_ dt );
			void render();

			//Generates a cube mesh and registers it
			BMesh::RID loadCubeMesh( std::string const &name, float size, bool wNormals = false ) {
				return mMeshManager.load( name, mMeshGenerator.buildCube( size, wNormals ) );
			};

		private:
			BRenderer mRenderer;
			BMeshManager mMeshManager;
			BObjMeshManager mObjMeshManager;
			BMeshGenerator mMeshGenerator;

			//TODO: Send camera info to the renderer
			BCamera *mActiveCamera;

			BSceneManager mSceneManager;
	};

};

using namespace Bolts;

void BSystem::init()
{
}

void BSystem::update( float_ dt )
{
	mSceneManager.updateNodes();

	std::set< BSceneManager::nodePointer > activeNodes;

	mSceneManager.fetchAllNodes( activeNodes );

	for ( std::set< BSceneManager::nodePointer >::iterator i = activeNodes.begin(); i != activeNodes.end(); i++ ) {
		if ( ( *i )->getObject() ) {
			( *i )->getObject()->update( dt );
		}
	}
}

void BSystem::render()
{
	mRenderer.setViewMatrix( getViewMatrix() );

	mRenderer.beforeRender();
	std::set< BSceneManager::nodePointer > activeNodes;

	mSceneManager.fetchAllNodes( activeNodes );

	for ( std::set< BSceneManager::nodePointer >::iterator i = activeNodes.begin(); i != activeNodes.end(); i++ ) {
		if ( ( *i )->getObject() ) {
			( *i )->getObject()->render();
		}
	}

	mRenderer.afterRender();
}

void BSystem::renderDebugAxes( const BNode3D &node )
{
	RIDType xmesh = mMeshManager.hashString( "BDB_XArrow" );
	RIDType ymesh = mMeshManager.hashString( "BDB_YArrow" );
	RIDType zmesh = mMeshManager.hashString( "BDB_ZArrow" );
	if ( mRenderer.meshLoaded( xmesh ) && mRenderer.meshLoaded( ymesh ) && mRenderer.meshLoaded( zmesh ) ) {
		//BMaterial *mat = mRenderer.newMaterial( "Basic");

		//mat->setParam( "vColor", vec4( 1., 0., 0., 1.));
		//mRenderer.setMaterial( mat);
		mRenderer.renderMesh( xmesh, node );

		//mat->setParam( "vColor", vec4( 0., 1., 0., 1.));
		//mRenderer.setMaterial( mat);
		mRenderer.renderMesh( ymesh, node );

		//mat->setParam( "vColor", vec4( 0., 0., 1., 1.));
		//mRenderer.setMaterial( mat);
		mRenderer.renderMesh( zmesh, node );
		//delete mat;
	} else {
		mMeshManager.load( "BDB_XArrow", mMeshGenerator.buildCube( 2.0f ) ); //mMeshGenerator.buildArrow('x', 2.));
		mMeshManager.load( "BDB_YArrow", mMeshGenerator.buildArrow( 'y', 2. ) );
		mMeshManager.load( "BDB_ZArrow", mMeshGenerator.buildArrow( 'z', 2. ) );
	}
}