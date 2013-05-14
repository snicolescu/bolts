#include <btBulletDynamicsCommon.h>
#include "BNode.h"

class BMotionState : public btMotionState {
	public:
		BMotionState( const btTransform &trans, Bolts::BNode3D *node ): mTrans( trans ), mNode( node ) {}

		virtual ~BMotionState() {
		}

		void setNode( Bolts::BNode3D *node ) {
			mNode = node;
		}

		virtual void getWorldTransform( btTransform &worldTrans ) const {
			worldTrans = mTrans;
		}

		virtual void setWorldTransform( const btTransform &worldTrans ) {
			if( NULL == mNode ) {
				return; // silently return before we set a node
			}
			btQuaternion rot = worldTrans.getRotation();
			mNode->setRotation( quat( rot.w(), rot.x(), rot.y(), rot.z() ) );
			btVector3 pos = worldTrans.getOrigin();
			mNode->setPosition( vec3( pos.x(), pos.y(), pos.z() ) );
		}

	protected:
		Bolts::BNode3D *mNode;
		btTransform mTrans;
};