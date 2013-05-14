#pragma once

#include "BGlobals.h"
#include "BEManager.h"
#include "BMesh.h"
#include <string>

namespace Bolts {
	class BMeshGenerator {
		public:
			//TODO: Texture coordinates are generated incorrectly
			static intrusive_ptr<BMesh>::t buildCube( float size, bool wNormals = false );

			/*	square_size is the size of each grid square
			 *	size is the number of squares
			 */
			intrusive_ptr<BMesh>::t buildGrid( float square_size, uint_ size );

			/*
			 *	Builds a very simple arrow, usually used for debugging. The arrow extends along the x axis
			 */
			intrusive_ptr<BMesh>::t buildArrow( char dir, float length, float width = 0.5f );
	};
};