#pragma once

#include "BGlobals.h"
#include "BSystem.h"
#include "BObject.h"
#include "BNode.h"

namespace Bolts {
	class BStaticMesh: public BObject {
		public:
			BStaticMesh( BSystem &engine, BNode3D *node, RIDType mesh ): mSystem( engine ), mMesh( mesh ), mNode( NULL ) {
				attachTo( node );
			};
			~BStaticMesh() {
				mNode->setObject( NULL );
			};

			void update( float_ dt ) {};
			void render();
			void nodeDetached() {
				mNode = NULL;
			};

			void attachTo( BNode3D *node ) {
				if( node ) {
					node->setObject( this );
				}
				mNode = node;
			};

			void setMesh( RIDType newMesh ) {
				mMesh = newMesh;
			};
			void setMaterial( BMaterial *mat ) {
				mMaterial = mat;
			};

		private:
			BNode3D *mNode;
			BMaterial *mMaterial;
			BSystem &mSystem;
			RIDType mMesh;
	};
};

using namespace Bolts;

void BStaticMesh::render()
{
	if ( mNode != NULL ) {
		mSystem.getRenderer().setMaterial( mMaterial );
		mSystem.getRenderer().renderMesh( mMesh, *mNode );
	} else {
		BLogger::error( "Object doesn't have a system or a node" );
	}
}