#include "BMesh.h"

using namespace Bolts;

BMesh::BMesh( size_t numV ): mPrimitiveType( B_MP_TRIANGLES ), mNumVertices( numV ), mNumIndices( 0 )
{
	mData.resize( B_MD_LAST );
	mDataInfo.hasData.resize( B_MD_LAST, false );
	mDataInfo.buffersUsed = 0;
};

void BMesh::loadIndexData( const std::vector< uint_ > &from )
{
	assert( mNumIndices == 0 );
	mIndexData.reserve( from.size() );
	mNumIndices = from.size();
	mDataInfo.buffersUsed++;
	mIndexData.assign( from.begin(), from.end() );
}

const std::vector< float_ > &BMesh::getData( B_MESH_DATA dataType ) const
{
	return mData[dataType];
}