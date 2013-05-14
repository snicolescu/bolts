#ifndef GPUVertexBufferBase_h__
#define GPUVertexBufferBase_h__

#include "Core/types.h"
#include "Core/SmartPointers.h"

#include <memory>
#include <boost/container/flat_map.hpp>
#include <boost/noncopyable.hpp>

namespace Bolts {
	namespace Rendering {

		enum AttributeSemantic {
		    BAS_POSITION = 0U, BAS_NORMAL, BAS_COLOR, BAS_TEXTURE0, _BAS_LAST_BUILTIN
		};

		enum BufferUsageHint {
			BUH_STATIC_DRAW, BUH_STATIC_READ, BUH_STATIC_COPY, 
			BUH_DYNAMIC_DRAW, BUH_DYNAMIC_READ, BUH_DYNAMIC_COPY, 
			BUH_STREAM_DRAW, BUH_STREAM_READ, BUH_STREAM_COPY
		};

		enum GPUVarType {
		    BGVT_FLOAT = 0, BGVT_HALF_FLOAT, BGVT_I8, BGVT_U8, BGVT_I16, BGVT_U16, BGVT_I32, BGVT_U32, _BGVT_LAST
		};
		const uint8_t GPUVarSize[_BGVT_LAST] = { sizeof( float ), 2, 1, 1, 2, 2, 4, 4};

		enum GPUStreamSemantic {
		    BGSS_INVALID = 0, BGSS_POSITION, BGSS_NORMALS, BGSS_COLOR0, BGSS_COLOR1, BGSS_TEXTURE0, BGSS_TEXTURE_1, BGSS_TEXTURE2, BGSS_TEXTURE3, _BGSS_LAST_SEMANTIC
		};

		class GPUBufferDescription {
			public:
				//TODO USABILITY: Auto-compute offset
				struct StreamDesc {
					StreamDesc(): semantic(BGSS_INVALID),attributeType(BGVT_FLOAT),componentNum(0),offset(0) {}


					StreamDesc( GPUStreamSemantic sem, GPUVarType var_t, uint8_t numComp, uint8_t off)
					{
						semantic = sem;
						attributeType = var_t;
						componentNum = numComp;
						offset = off;
					}

					bool operator< ( const StreamDesc &other ) {
						return semantic < other.semantic;
					}

					GPUStreamSemantic	semantic;
					GPUVarType			attributeType;
					uint8_t				componentNum;
					uint8_t				offset;
				};
				typedef ::boost::container::flat_map< GPUStreamSemantic, StreamDesc > stream_map_t;

				GPUBufferDescription(): m_stride( 0 ),m_strideDirty(true), m_streamMap() {}

				bool IsBlank() const{
					return ( m_streamMap.size() == 0 );
				}
				//Adds a new stream to the buffer's description. Asserts when another stream with the same semantic already exists.
				void AddStream( const StreamDesc &newStream );
				//Adds a new stream to the buffer's description, replacing streams with the same semantic.
				void ReplaceStream( const StreamDesc &newStream );

				uint8_t GetStride() const;
				//Returns the first StreamDescription in the map. Descriptions are guaranteed to be ordered by GPUStreamSemantic
				const stream_map_t& GetStreams() const;
			private:
				//TODO BUG: Doesn't take into account component offset.
				//			Data that isn't tightly packed generates wrong stride
				void RecomputeStride() const;

				stream_map_t	m_streamMap;
				mutable uint8_t m_stride;
				mutable bool	m_strideDirty;
		};

		//TODO: Add proper copy constructor !
		//TODO PERF: Add a way to only change a portion of the data
		//			Maybe expose the pointer ?
		//			Or add an update function
		class GPUVertexBufferBase:public SmallRefCountT{
			public:
				//By default it takes ownership of the data and will free it on destruction
				//	Otherwise it will make a copy and memcpy the data from one place to the other
				//NOTE: If the previous data buffer size matches the current one no memory allocation occurs
				//Returns: True on success. False of failure( only when making a copy)
				bool						SetData(BufferUsageHint usage, void *source, size_t dataSize, bool makeCopy = true);
				//Returns the size of the data in bytes
				size_t						GetByteSize() const{
					return m_bufferSize;
				}

				void						SetNumberOfVertices(uint32_t vertNum){
					m_numVertices = vertNum;
				}

				uint32_t					GetNumVertices() const{
					return m_numVertices;
				}

				GPUBufferDescription&		GetDescription() {
					return m_description;
				}
				const GPUBufferDescription&	GetDescription() const {
					return m_description;
				}

				bool						IsEmpty() const {
					return ( m_bufferSize == 0 );
				}
				bool						IsDirty() const {
					return m_isDirty;
				}

			protected:
				//Can only be constructed by children
											GPUVertexBufferBase();
											GPUVertexBufferBase(const GPUBufferDescription &desc);
											~GPUVertexBufferBase();

				void						ClearData();

				//TODO: Replace pointers with a proper container
				void					*m_bufferData;
				size_t					m_bufferSize; //in bytes
				uint32_t				m_numVertices;
				GPUBufferDescription	m_description;
				BufferUsageHint			m_usage;
				mutable bool			m_isDirty;
		};

	};
};
#endif // GPUVertexBufferBase_h__

