#include "BGlobals.h"
#include "BLogger.h"
#include "BNode.h"

//Uses Bullet Physics
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btAlignedObjectArray.h>
#include "BMotionState.h"

namespace Bolts {
	class BPhysics {
		public:
			BPhysics(): mCollisionShapes() {};

			//Initializes the physics world and adds a ground object
			void init();
			void step( float dt , uint_ maxsteps = 7 );
			void exit();

			//Adds a box to the physics simulation and returns the rigid body
			//NOTE: The node's position and rotation will automatically be updated by the physics engine
			btRigidBody *addBox( float size, Bolts::BNode3D *node );

		private:
			btDynamicsWorld *mDynamicsWorld;
			btBroadphaseInterface	*mBroadphase;
			btCollisionDispatcher	*mDispatcher;
			btConstraintSolver	*mSolver;
			btDefaultCollisionConfiguration *mCollisionConfiguration;

			btAlignedObjectArray<btCollisionShape *>	mCollisionShapes;
	};
};