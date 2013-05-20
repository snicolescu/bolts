#pragma once

#pragma warning( push, 3)
#include <boost/intrusive_ptr.hpp>
#pragma warning( pop)

#include "types.h"

namespace Bolts {

	template<typename T>
	struct IntrusivePtr {
		typedef boost::intrusive_ptr<T> t;
	};

	template< typename I>
	class RefCountedType {
		public:
			RefCountedType(): m_NumRefrences( 0 ) {};

			void onReferenceAdd() const {
				++m_NumRefrences;
			};

			//Returns true if the instance should be deleted
			bool onReferenceRelease() const {
				return ( --m_NumRefrences == 0 );
			}

		private:
			mutable I m_NumRefrences;
	};

	typedef RefCountedType< uint32_t > BigRefCountT;
	typedef RefCountedType< uint16_t > RefCountT;
	typedef RefCountedType< uint8_t >  SmallRefCountT;

};

namespace boost {
	//Intrusive pointer related
	template<typename T>
	inline void intrusive_ptr_add_ref( const T *expr )
	{
		expr->onReferenceAdd();
	};

	template<typename T>
	inline void intrusive_ptr_release( const T *expr )
	{
		if( expr->onReferenceRelease() ) {
			delete expr;
		}
	};
};