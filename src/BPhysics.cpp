#include "BPhysics.h"

void Bolts::BPhysics::init()
{
	///collision configuration contains default setup for memory, collision setup
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	mDispatcher = new	btCollisionDispatcher( mCollisionConfiguration );
	mBroadphase = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver *mSolver = new btSequentialImpulseConstraintSolver;

	mDynamicsWorld = new btDiscreteDynamicsWorld( mDispatcher, mBroadphase, mSolver, mCollisionConfiguration );

	mDynamicsWorld->setGravity( btVector3( 0, -10, 0 ) );

	///create the ground
	btCollisionShape *groundShape = new btBoxShape( btVector3( btScalar( 50. ), btScalar( 50. ), btScalar( 50. ) ) );

	mCollisionShapes.push_back( groundShape );

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin( btVector3( 0, -50, 0 ) );

	{
		btScalar mass( 0. );
		btVector3 localInertia( 0, 0, 0 );

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState *myMotionState = new btDefaultMotionState( groundTransform );
		btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, myMotionState , groundShape, localInertia );
		btRigidBody *body = new btRigidBody( rbInfo );

		//add the body to the dynamics world
		mDynamicsWorld->addRigidBody( body );
	}
}

void Bolts::BPhysics::step( float dt, uint_ steps )
{
	mDynamicsWorld->stepSimulation( dt, steps );
}

void Bolts::BPhysics::exit()
{

	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for ( i = mDynamicsWorld->getNumCollisionObjects() - 1; i >= 0 ; i-- ) {
		btCollisionObject *obj = mDynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody *body = btRigidBody::upcast( obj );
		if ( body && body->getMotionState() ) {
			delete body->getMotionState();
		}
		mDynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for ( int j = 0; j < mCollisionShapes.size(); j++ ) {
		btCollisionShape *shape = mCollisionShapes[j];
		delete shape;
	}
	mCollisionShapes.clear();

	delete mDynamicsWorld;

	delete mSolver;

	delete mBroadphase;

	delete mDispatcher;

	delete mCollisionConfiguration;
}

btRigidBody *Bolts::BPhysics::addBox( float size, Bolts::BNode3D *node )
{
	if ( !node ) {
		Bolts::BLogger::error( "[Physics] Add Box Was passed a null node" );
		return NULL;
	}

	Bolts::BLogger::print( "[Physics] Added Box" );
	btCollisionShape *boxShape = new btBoxShape( btVector3( size, size, size ) );
	mCollisionShapes.push_back( boxShape );

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setOrigin( btVector3( node->getPosition().x, node->getPosition().y, node->getPosition().z ) );
	btScalar	mass( 1.f );
	btVector3 localInertia( 0, 1., 0 );
	boxShape->calculateLocalInertia( mass, localInertia );

	BMotionState *ms = new BMotionState( startTransform, node );
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, ms, boxShape, localInertia );
	btRigidBody *body = new btRigidBody( rbInfo );
	mDynamicsWorld->addRigidBody( body );

	return body;
}