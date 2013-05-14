#pragma once

#include "BGlobals.h"

namespace Bolts {
	class BObject {
		public:
			virtual void update( float_ ) = 0;
			virtual void render() = 0;
			//Called when another object is bound to this object's node
			virtual void nodeDetached() = 0;
	};
};