#pragma once
#include "BGlobals.h"
#include "BNode.h"
#include <map>
#include <set>

namespace Bolts {

	class BSimpleSceneManager {
		public:
			typedef BNode3D::my_type *nodePointer;

			BSimpleSceneManager(): mRoot( new BNode3D ) {};

			void	setSceneSize( float_ width, float_ length, float_ height ) {};

			void	addNode( nodePointer node ) {
				mRoot->addChild( node );
			};
			void	addToNode( nodePointer parent, nodePointer child ) {
				if ( parent ) {
					parent->addChild( child );
				}
			};
			void	removeNode( nodePointer child );

			void	updateNodes();

			//This is temporary until I implement actual queries.
			void	fetchAllNodes( std::set< nodePointer > &putHere );
		private:
			nodePointer mRoot;
	};
};