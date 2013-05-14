#include "BSimpleSceneManager.h"

using namespace Bolts;

void	BSimpleSceneManager::removeNode( nodePointer node )
{
	node->getParent()->removeChild( node );
}

void	BSimpleSceneManager::updateNodes()
{
	mRoot->updateChildren();
}

void	BSimpleSceneManager::fetchAllNodes( std::set< nodePointer > &putHere )
{
	mRoot->addToList( putHere );
}