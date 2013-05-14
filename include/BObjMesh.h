#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <sstream>
#include <fstream>

#include "BGlobals.h"
#include "BLogger.h"

namespace Bolts {
	/*	Data will be kept separated
	 *	Only supports triangle faces
	 *	Does not support negative indices
	 *
	*/
	struct BObjMesh: public ref_counted_type {
		typedef RIDType RID;
		//TODO: Also keep primitive type info

		struct ObjVertex {
			ObjVertex( float f1, float f2, float f3 ): x( f1 ), y( f2 ), z( f3 ) {}
			float_ x, y, z;
		};

		struct ObjNormal {
			ObjNormal(): x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
			ObjNormal( float f1, float f2, float f3 ): x( f1 ), y( f2 ), z( f3 ) {};

			float_ x, y, z;
		};

		struct ObjTexCoord {
			ObjTexCoord(): u( 0.0f ), v( 0.0f ) {}
			ObjTexCoord( float f1, float f2 ): u( f1 ), v( f2 ) {}

			float_ u, v;
		};

		struct ObjFace {
			ObjFace() : v1( 0 ), v2( 0 ), v3( 0 ) {};
			ObjFace( int _v1, int _v2, int _v3 ) : v1( _v1 ), v2( _v2 ), v3( _v3 ) {};

			uint_ v1, v2, v3;
		};

		BObjMesh(): mHasNormals( false ), mHasTexCoords( false ) {};

		bool mHasNormals;
		bool mHasTexCoords;

		std::vector< ObjVertex > mVerts;
		std::vector< ObjNormal > mNormals;
		std::vector< ObjTexCoord > mTexCoords;

		std::vector< ObjFace > mFaces;

		//Return false if loading fails. It will print error details to the log.
		bool loadFromFile( std::string filename );
	};
};

std::ostream &operator<< ( std::ostream &os, const Bolts::BObjMesh::ObjVertex &vert );
std::ostream &operator<< ( std::ostream &os, const Bolts::BObjMesh::ObjNormal &vert );
std::ostream &operator<< ( std::ostream &os, const Bolts::BObjMesh::ObjFace &vert );