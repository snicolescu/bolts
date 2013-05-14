#pragma once
#include "BGlobals.h"
#include "BIDManager.h"
#include "BObject.h"
#include "B3DTransform.h"

#include <vector>
#include <algorithm>
#include <set>

namespace Bolts {
	template<class PositionData>
	class BNode: public PositionData {
		public:
			typedef BNode<PositionData> my_type;

			BNode( bool staticNode = false ): PositionData( staticNode ), mParent( NULL ), mObject( NULL ) {};
			~BNode() {};

			my_type	&setParent( my_type *parent );
			my_type	*getParent();
			bool		hasParent();

			my_type	&addChild( my_type *child );
			my_type	&removeChild( my_type const *child );
			std::vector< my_type * > const &getChildren() {
				return mChildren;
			};
			//	Calls updateAbsData() on all the children and then calls update children on them
			void		updateChildren();

			//Adds this node and it's subtree to the specified list
			void		addToList( std::set< my_type * > &list );

			void updateAbsData();

			BObject *getObject() {
				return mObject;
			};
			void setObject( BObject *newObj ) {
				if( mObject ) {
					mObject->nodeDetached();
				}
				mObject = newObj;
			};

		protected:
			static BIDManager mIdManager;
			BObject *mObject;

			my_type *mParent;
			std::vector< my_type * > mChildren;
	};

	//TODO: hide this class somehow
	class BNode3Ddata {
		public:
			BNode3Ddata( bool staticNode ): mIsStatic( staticNode ), mLocalTransform(), mAbsRotation(), mAbsPosition() {};

			void setPosition( vec3 pos ) {
				mLocalTransform.mPosition = pos;
			};
			vec3 getPosition() const {
				return mLocalTransform.mPosition;
			};

			void setRotation( quat rot ) {
				mLocalTransform.mRotation = rot;
			};
			quat getRotation() const {
				return mLocalTransform.mRotation;
			};

			vec3 getAbsPosition() const {
				return mAbsPosition;
			};
			quat getAbsRotation() const {
				return mAbsRotation;
			};

			bool isStatic() {
				return mIsStatic;
			};

		protected:
			bool mIsStatic;
			B3DTransform mLocalTransform;
			vec3 mAbsPosition;
			quat mAbsRotation;

			void updateAbsData( vec3 const &parentPos, quat const &parentRot ) {
				mAbsPosition = parentPos + ( parentRot * mLocalTransform.mPosition );
				mAbsRotation = parentRot + mLocalTransform.mRotation;
			};

			void updateAbsData() {
				mAbsPosition = mLocalTransform.mPosition;
				mAbsRotation = mLocalTransform.mRotation;
			};
	};

	typedef BNode<BNode3Ddata> BNode3D;

	//template<BNode3Ddata>
	//BIDManager BNode<BNode3Ddata>::mIdManager = BIDManager();
};

using namespace Bolts;

template<class PositionData>
BNode<PositionData> &BNode<PositionData>::setParent( my_type *parent )
{
	if ( mParent ) {
		mParent->removeChild( this );
	}
	mParent = parent;
	return *this;
}

template<class PositionData>
BNode<PositionData> *BNode<PositionData>::getParent()
{
	return mParent;
}

template<class PositionData>
bool	BNode<PositionData>::hasParent()
{
	return ( mParent != NULL );
}

template<class PositionData>
BNode<PositionData> &BNode<PositionData>::addChild( my_type *child )
{
	if ( child != NULL ) {
		mChildren.push_back( child );
		child->setParent( this );
	}
	return *this;
}

template<class PositionData>
BNode<PositionData> &BNode<PositionData>::removeChild( my_type const *child )
{
	typename std::vector< BNode<PositionData>* >::iterator it = find( mChildren.begin(), mChildren.end(), child );
	if ( it != mChildren.end() ) {
		mChildren.erase( it );
	}
	return *this;
}

template<class PositionData>
void BNode<PositionData>::updateAbsData()
{
	if ( hasParent() ) {
		PositionData::updateAbsData( mParent->getAbsPosition(), mParent->getAbsRotation() );
	} else {
		PositionData::updateAbsData();
	}
};

template<class PositionData>
void BNode<PositionData>::updateChildren()
{
	for ( uint_ i = 0; i < mChildren.size(); ++i ) {
		mChildren[i]->updateAbsData();
		mChildren[i]->updateChildren();
	}
}

template<class PositionData>
void BNode<PositionData>::addToList( std::set< my_type * > &list )
{
	list.insert( this );

	for ( uint_ i = 0; i < mChildren.size(); i++ ) {
		mChildren[i]->addToList( list );
	}
}

