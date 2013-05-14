#include "BObjMesh.h"

using namespace Bolts;
using std::string;
using std::istringstream;
using std::ifstream;

bool BObjMesh::loadFromFile( string filename )
{
	ifstream input( filename.c_str() );
	string buffer;
	std::vector< ObjTexCoord > tempCoords;
	std::vector< ObjNormal > tempNormals;

	bool hasNormals = false, hasTexCoords = false;

	if ( !input ) {
		BLogger::error( "[OBJMESH] Error on load. File not found: " + filename );
		return false;
	}

	while ( getline( input, buffer ) ) {
		std::istringstream line( buffer );
		string line_t;

		if ( buffer.substr( 0, 2 ) == "vn" ) {
			hasNormals = true;
			double  f1, f2, f3;
			line >> line_t >> f1 >> f2 >> f3;
			tempNormals.push_back( ObjNormal( ( float )f1, ( float ) f2, ( float )f3 ) );
		} else if ( buffer.substr( 0, 2 ) == "vt" ) {
			hasTexCoords = true;
			float f1, f2;
			line >> line_t >> f1 >> f2;
			tempCoords.push_back( ObjTexCoord( f1, f2 ) );
		} else if ( buffer.substr( 0, 1 ) == "v" ) {
			float f1, f2, f3;
			line >> line_t >> f1 >> f2 >> f3;
			mVerts.push_back( ObjVertex( f1, f2, f3 ) );
		} else if ( buffer.substr( 0, 1 ) == "f" ) {
			if ( hasNormals ) {
				mNormals.resize( mVerts.size() );
			}
			if ( hasTexCoords ) {
				mTexCoords.resize( mVerts.size() );
			}

			string element;
			line >> line_t;
			int ind = 0;
			uint_ verts[4];

			while ( line >> element && ind < 4 ) {
				string::size_type firstSlash = element.find( '/' );
				string::size_type secondSlash = element.find( '/', firstSlash + 1 );

				if ( firstSlash != string::npos ) {
					istringstream stream( element.substr( 0, firstSlash ) );
					stream >> verts[ ind ];
					--verts[ ind ];//Indices are 0 based

					if ( secondSlash == firstSlash + 1 ) {
						//No texture data
						stream = istringstream( element.substr( secondSlash + 1 ) );

						//Copy normal data for the specified vertex
						int normInd;
						stream >> normInd;
						mNormals[ verts[ind] ] = tempNormals[ normInd - 1 ];
					} else {
						if ( secondSlash == string::npos ) {
							//No Normal data
							stream = istringstream( element.substr( firstSlash + 1 ) );

							//Copy texture data for the specified vertex
							uint_ texInd;
							stream >> texInd;
							mTexCoords[ verts[ind] ] = tempCoords[ texInd - 1 ];
						} else {
							//Both normal and tex data
							stream = istringstream( element.substr( firstSlash + 1, secondSlash - firstSlash - 1 ) );
							//Copy texture data for the specified vertex
							uint_ texInd;
							stream >> texInd;
							mTexCoords[ verts[ind] ] = tempCoords[ texInd - 1 ];

							stream = istringstream( element.substr( secondSlash + 1 ) );
							//Copy normal data for the specified vertex
							uint_ normInd;
							stream >> normInd;
							mNormals[ verts[ind] ] = tempNormals[ normInd - 1 ];
						}
					}
				} else {
					//No Normal or Texture Indices
					if ( hasNormals ) {
						mNormals = tempNormals;
					}
					if ( hasTexCoords ) {
						mTexCoords = tempCoords;
					}

					istringstream stream( element );
					stream >> verts[ ind ];
					--verts[ind];
				}

				++ind;
				continue;
			}

			if ( ind < 3 ) {
				BLogger::error( "[OBJMESH] Error on load. Face doesn't have 3 vertices defined. Starting with " + verts[0] );
				return false;
			}

			mFaces.push_back( ObjFace() );
			mFaces.back().v1 = verts[0];
			mFaces.back().v2 = verts[1];
			mFaces.back().v3 = verts[2];
			if ( ind == 4 ) {
				mFaces.push_back( ObjFace() );
				mFaces.back().v1 = verts[0];
				mFaces.back().v2 = verts[2];
				mFaces.back().v3 = verts[3];
			}
		}
	}

	mHasNormals = hasNormals;
	mHasTexCoords = hasTexCoords;

	input.close();
	return true;
}

std::ostream &operator<< ( std::ostream &os, const BObjMesh::ObjVertex &vert )
{
	os << " V: " <<  vert.x << " " << vert.y << " " << vert.z;
	return os;
}

std::ostream &operator<< ( std::ostream &os, const BObjMesh::ObjNormal &norm )
{
	os << " N: " <<  norm.x << " " << norm.y << " " << norm.z;
	return os;
}

std::ostream &operator<< ( std::ostream &os, const BObjMesh::ObjFace &face )
{
	os << " f: " <<  face.v1 << " " << face.v2 << " " << face.v3;
	return os;
}