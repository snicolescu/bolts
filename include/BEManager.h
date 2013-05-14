#pragma once
#include <boost/shared_ptr.hpp>
#include <map>

namespace Bolts {
	//TODO: Use the BIDManager Class
	template <typename T>
	class BEManager {
		public:
			typedef typename T::RID RID;
			typedef boost::shared_ptr<T> RPtr;

			T &addR( RID rid, RPtr res ) {
				mResList[rid] = res;
				return ( *res );
			}

			T &getR( RID rid ) {
				return ( *mResList[rid] );
			}

			bool isLoaded( RID rid ) const {
				return ( mResList.count( rid ) == 1 );
			}

		protected:
			~BEManager() {};

			std::map< RID, RPtr > mResList;
	};
};