#pragma once
#include <boost/crc.hpp>

#include "BGlobals.h"
#include "BConfig.h"
#include "BMesh.h"
#include "BObjMesh.h"
#include <unordered_map>
#include <string>

namespace Bolts {
	// All resources should inherit from the ref_counted class ( defined in the BGlobals.h header)
	//Template parameters:
	//	T : Resource Type
	// Can only be constructed with a System reference
	//TODO: Switch back to regular maps
	//TODO: Add logging and error reporting.
	//TODO: See if maintaining a single HashObject is preferred( i.e. faster)
	template <typename T>
	class BResourceManagerT {
		public:
			typedef typename T::RID RID;//RID type for T
			typedef typename intrusive_ptr<T>::t RPtr;//Pointer to a T

			typedef boost::crc_32_type BHashObject;

			BResourceManagerT( BRenderer &rnd ): mRenderer( rnd ) {};

			//Registers a resource along with it's RID to this manager.
			//NOTE This overloads any previous res loaded in this slot.
			//NOTE If a hashing collision occurs it will go unnoticed. But this is very very unlikely (using CRC32).
			typename RID load( std::string const &rname, RPtr res );
			void load( RID rid, RPtr res );

			//Removes and deletes this resource
			void unload( RID rid );

			//Retrieves the resource from the list of loaded resources.
			//Returns NULL if the resource is not found.
			RPtr get( RID rid );

			//Returns true if a resource with the specified RID is registered.
			bool isLoaded( RID rid ) const {
				return ( mResList.count( rid ) == 1 );
			};

			//Returns the hash value of the string
			RIDType hashString( const std::string &str ) {
				BHashObject hash;
				hash.process_bytes( str.c_str(), str.size() );
				return hash.checksum();
			}

		protected:
			BRenderer &mRenderer;

			std::unordered_map< RID, RPtr > mResList;
	};

	typedef BResourceManagerT< BMesh > BMeshManager;
	typedef BResourceManagerT< BObjMesh > BObjMeshManager;
};

using namespace Bolts;

template <typename T>
typename BResourceManagerT< T >::RID BResourceManagerT< T >::load( std::string const &rname, RPtr res )
{
	BHashObject hash;
	hash.process_bytes( rname.c_str(), rname.size() );

	load( hash.checksum(), res );
	return hash.checksum();
};


//		BMesh
template <>
void BResourceManagerT< BMesh >::load( RID rid, RPtr res )
{
	mResList[rid] = res;
	mRenderer.loadMesh( rid, *res );
};

template <>
void BResourceManagerT< BMesh >::unload( RID rid )
{
	mResList.erase( rid );
	mRenderer.unloadMesh( rid );
};

//		BObjMesh
template <>
void BResourceManagerT< BObjMesh >::load( RID rid, RPtr res )
{
	mResList[rid] = res;
	mRenderer.loadMesh( rid, *res );
};

template <>
void BResourceManagerT< BObjMesh >::unload( RID rid )
{
	mResList.erase( rid );
	mRenderer.unloadMesh( rid );
};

template <typename T>
typename BResourceManagerT< T >::RPtr BResourceManagerT< T >::get( RID rid )
{
	if ( isLoaded( rid ) ) {
		return mResList[rid];
	} else {
		return intrusive_ptr<T>();
	}
};
