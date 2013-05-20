#pragma once
#include <cstdlib>
#include <assert.h>

#ifdef _DEBUG
#define BOLTS_ASSERT(expr) assert( expr );
#else
#define BOLTS_ASSERT(expr)
#endif

#define SAFE_DEL( x ) if (x){ \
		delete x; \
		x = NULL; \
	}

#define SAFE_DEL_ARRAY( x ) if (x){ \
		delete[] x; \
		x = NULL; \
	}

#define SAFE_FREE( x ) if (x){ \
		free(x); \
		x = NULL; \
	}