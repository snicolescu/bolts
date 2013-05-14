#pragma once
#include <memory>
#include <vector>

#include "BGlobals.h"
#include "BNode.h"

#include "BLogger.h"

namespace Bolts {

	template<class Value>
	class BOctree {
		public:
			//TODO: Add a destructor to free memory
			//NOTE: Memory leaks on destruction
			BOctree( float_ size, vec3 cnt = vec3() ): center( cnt ), limits( size ), value( NULL ), numChildren( 0 ), parent( NULL ) {
				corners.resize( 8 );
			};
			BOctree( vec3 size, vec3 cnt = vec3() ): center( cnt ), limits( size ), value( NULL ), numChildren( 0 ), parent( NULL ) {
				corners.resize( 8 );
			};

			void pushItem( vec3 const &key, Value const *val );
			//Removes the node found at key if it's value equals val
			void removeNode( vec3 const &key, Value const *val );

			void print( byte_t depth = 0 );

		private:
			Value const *value;
			vec3 key;

			vec3 center;
			vec3 limits;
			byte_t numChildren;

			std::vector< BOctree<Value>* > corners;
			BOctree<Value> *parent;

			void	addChild( byte_t corner, Value const *val, vec3 const &key );
			void	deleteChild( byte_t corner );
			void	deleteChildren();

			byte_t getOctant( vec3 const &point ) const;

			inline bool isLeaf() const {
				return ( numChildren == 0 );
			};
			inline bool	hasChild( byte_t corner ) const {
				return ( corners[corner] != NULL );
			};

			BOctree<Value> *findRec( vec3 const &key, Value const &val );
	};

	/*		OCTREE public functions */

	template<class Value>
	void BOctree<Value>::pushItem( vec3 const &pos, Value const *newval )
	{
		//TODO: Check to see if this new position is too close to the leaf node's position
		//Right now, two ovelapping points crash the game
		if ( !isLeaf() ) {
			byte_t oct = getOctant( pos );
			if ( !hasChild( oct ) ) {
				addChild( oct, newval, pos );
			} else {
				corners[oct]->pushItem( pos, newval );
			}
			return;
		}

		if ( isLeaf() && value == NULL ) {
			value = newval;
			key = pos;
			return;
		}

		//Leaf node with value, split
		byte_t oct = getOctant( pos );
		if ( !hasChild( oct ) ) {
			addChild( oct, newval, pos );
		} else {
			corners[oct]->pushItem( pos, newval );
		}

		oct = getOctant( key );
		if ( !hasChild( oct ) ) {
			addChild( oct, value, key );
		} else {
			corners[oct]->pushItem( key, value );
		}
		value = NULL;
	}

	template<class Value>
	void BOctree<Value>::removeNode( vec3 const &key, Value const *val )
	{

	}

	/*		OCTREE inner workings	*/

	template<class Value>
	BOctree<Value> *BOctree<Value>::findRec( vec3 const &pos, Value const &val )
	{
		if ( !isLeaf() ) {
			byte_t oct = getOctant( pos );
			if ( hasChild( oct ) ) {
				return corners[oct]->findRec( pos, val );
			} else {
				return NULL;
			}
		} else {
			if ( value == val ) {
				return this;
			} else {
				return NULL;
			}
		}
	}

	template<class Value>
	void BOctree<Value>::addChild( byte_t oct, Value const *val, vec3 const &key )
	{
		vec3 newLimit( limits );
		newLimit /= 2;
		vec3 newCenter( center.x + newLimit.x * ( 1 + -2 * ( oct & 4 ) ),
		                center.y + newLimit.y * ( 1 + -2 * ( oct & 2 ) ),
		                center.z + newLimit.z * ( 1 + -2 * ( oct & 1 ) ) );
		corners[oct] = new BOctree<Value>( newLimit, newCenter );
		corners[oct]->parent = this;
		corners[oct]->value = val;
		corners[oct]->key = key;
		++numChildren;
	}

	template<class Value>
	void BOctree<Value>::deleteChild( byte_t ind )
	{
		if ( corners[ ind ] != NULL ) {
			return;
		}
		numChildren--;
		delete corners[ ind ];
		corners[ ind ] = NULL;
	}

	template<class Value>
	void BOctree<Value>::deleteChildren()
	{
		for ( int i = 0; i < 8; ++i ) {
			delete corners[i];
			corners[i] = NULL;
		}
		numChildren = 0;
	}

	template<class Value>
	byte_t BOctree<Value>::getOctant( vec3 const &point ) const
	{
		char oct = ( point.x <= center.x ) << 1;
		oct += ( point.y <= center.y ) << 1;
		oct += point.z <= center.z;
		return oct;
	}

	/*		OCTREE debugging functions	*/

	template<class Value>
	void BOctree<Value>::print( byte_t depth = 0 )
	{
		if ( isLeaf() ) {
			std::cout << ( int ) depth;
			for ( int i = 0; i < depth; ++i ) {
				std::cout << '-';
			}
			BLogger::print( key );
		} else {
			for ( int i = 0; i < 8; ++i ) {
				if ( hasChild( i ) ) {
					corners[i]->print( depth + 1 );
				}
			}
		}
	}

};