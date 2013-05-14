#ifndef GPUBuffer_h__
#define GPUBuffer_h__

#include "types.h"

class GPUBuffer {
	public:
		enum DataType { DT_INTEGER = 0, DT_SHORT, DT_FLOAT};
		enum DataUsageHint { DU_STATIC, DU_DYNAMIC};

		struct DataDescription {
			DataType	type;
			uint8_t		count;//num of elements
			uint8_t		offset;//in Bytes ( used with interleaved data, otherwise 0)
		};

	private:

};

#endif// GPUBuffer_h__