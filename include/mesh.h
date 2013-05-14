#pragma once
#include <vector>
#include <algorithm>
#include <assert.h>
#include <cstring>

#include "logger.h"
#include "smart_pointers.h"

namespace Bolts {

	class Mesh: public normal_ref_count {
		public:
			enum MeshData { MD_INDEX = 0, MD_POSITION, MD_COLOR, MD_NORMAL, MD_TEX0, MD_COUNT};
			enum PrimitiveType { PT_TRIANGLES, PT_TRIANGLESTRIP, PT_LINES};

			Mesh();

			//Copies the data from the from[] vector into the specified data buffer
			template<typename T>
			void loadDataForBuffer( const std::vector<T> &from, B_MESH_DATA dataType );
			//TODO: Change index from uint_ to ushort_
			void loadIndexData( const std::vector< uint_ > &from );

			const std::vector< float_ > &getData( B_MESH_DATA dataType ) const;
			const std::vector< uint_ > &getIndexData() const {
				return mIndexData;
			};
			size_t getNumVertices() const {
				return mNumVertices;
			};
			size_t getNumIndices() const {
				return mNumIndices;
			};
			bool hasIndexData() const {
				return mNumIndices > 0;
			};
			DataInfo getDataInfo() const {
				return mDataInfo;
			};

			B_MESH_PRIMITIVE getPrimitiveType() const {
				return mPrimitiveType;
			};
			void setPrimitiveType( B_MESH_PRIMITIVE type ) {
				mPrimitiveType = type;
			};
		private:
			size_t mNumVertices;
			size_t mNumIndices;
			B_MESH_PRIMITIVE mPrimitiveType;

			DataInfo mDataInfo;

			std::vector< std::vector< float_ > > mData;
			std::vector< uint_ >	mIndexData;
	};
};

using namespace Bolts;

template<typename T>
void BMesh::loadDataForBuffer( const std::vector<T> &from, B_MESH_DATA bufferIndex )
{
	assert( !mDataInfo.hasData[bufferIndex] );
	mData[bufferIndex].reserve( from.size()* sizeof( T ) );
	mDataInfo.hasData[bufferIndex] = true;
	mDataInfo.buffersUsed++;
	mData[bufferIndex].assign( from.begin(), from.end() );
}