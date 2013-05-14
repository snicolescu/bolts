#pragma once
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <glm/glm.hpp>

#include "BGlobals.h"

namespace Bolts {

	class BRng {
		public:
			static BRng *get() {
				static BRng instance;
				return &instance;
			}

			float_ randFloat() {
				return ( float_ )rand() / ( float_ )RAND_MAX;
			};
			float_ randFloat( float_ min, float_ max ) {
				return ( min + ( max - min ) * randFloat() );
			};

			glm::vec3 randVec3() {
				return glm::vec3( randFloat(), randFloat(), randFloat() );
			};

		private:
			BRng() {
				srand( ( unsigned )time( 0 ) );
			};
			BRng( BRng const &ref ) {};
			BRng operator= ( BRng const &ref ) {};
	};
};