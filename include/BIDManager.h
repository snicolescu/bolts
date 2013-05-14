#pragma once
#include <stack>

namespace Bolts {
	template<typename idType>
	class BIDManagerTemplate {
		public:
			BIDManagerTemplate() {
				IDStack.push( 1 );
			};

			idType genId() {
				idType nextID = IDStack.top();
				IDStack.pop();
				if ( IDStack.size() == 0 ) {
					IDStack.push( nextID + 1 );
				}
				return nextID;
			};
			void idFreed( idType id ) {
				IDStack.push( id );
			};

		private:
			std::stack< idType > IDStack;
	};

	typedef BIDManagerTemplate<uint_> BIDManager;
};