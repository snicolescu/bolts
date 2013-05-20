#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef unsigned int u32;


static const u32 _m = 0x5bd1e995;
static const u32 _r = 24;

template <size_t LEN>
u32 MURMURHASH_LEN( u32 hash, const char *str, int idx )
{
	return MURMURHASH_LEN < LEN - 4 > (
	           MURMURHASH_LEN<4>( hash, str, idx ),
	           str,
	           idx + 4 );
}

template <>
u32 MURMURHASH_LEN<4>( u32 hash, const char *str, int idx )
{
#define _k  ((str[idx+0]) | ((str[idx+1])<<8) | ((str[idx+2]) << 16) | ((str[idx+3]) << 24))
	u32 k;
	k = _k * _m;
	k ^= ( k >> _r );
	k *= _m;
	return ( hash * _m ) ^ k;
}

template <>
u32 MURMURHASH_LEN<3>( u32 hash, const char *str, int idx )
{
	return ( hash ^ ( str[idx + 2] << 16 ) ^ ( str[idx + 1] << 8 ) ^ ( str[idx + 0] ) ) * _m;
}

template <>
u32 MURMURHASH_LEN<2>( u32 hash, const char *str, int idx )
{
	return ( hash ^ ( str[idx + 1] << 8 ) ^ ( str[idx + 0] ) ) * _m;
}

template <>
u32 MURMURHASH_LEN<1>( u32 hash, const char *str, int idx )
{
	return ( hash ^ ( str[idx + 0] ) ) * _m;
}

template <>
u32 MURMURHASH_LEN<0>( u32 hash, const char *str, int idx )
{
	return ( hash );
}

template <size_t LEN>
u32 MURMURHASH( u32 seed, const char *str )
{
	u32 h = MURMURHASH_LEN<LEN>( seed ^ LEN, str, 0 );
	h ^= h >> 13;
	h *= _m;
	h ^= h >> 15;
	return h;
}

u32 MurmurHash ( const void *key, u32 len, u32 seed )
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	// Initialize the hash to a 'random' value

	u32 h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const u32 *data = ( u32 * )key;

	while( len >= 4 ) {
		u32 k = *( u32 * )data;

		k *= _m;
		k ^= k >> _r;
		k *= _m;

		h *= _m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	// Handle the last few bytes of the input array

	switch( len ) {
		case 3:
			h ^= data[2] << 16;
		case 2:
			h ^= data[1] << 8;
		case 1:
			h ^= data[0];
			h *= _m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= _m;
	h ^= h >> 15;

	return h;
}

// this generates a hash from a dynamic string.
#define dynamic_murmurhash(str, seed)   \
	MurmurHash(str, (u32)strlen(str), seed)

// this generates an immediate hash with a literal string. Do NOT use with a dynamic string.
#define static_murmurhash(str, seed)    \
	MURMURHASH<sizeof(str)-1>(seed, str)


void main( int argc, char **argv )
{
	const char *str = argv[0];
	u32 hash = dynamic_murmurhash( str, 0 );


	// VS2005 Release should generate:
	// cmp	esi, -1146709846			; bba698aaH
	if ( hash == static_murmurhash( "blah", 0 ) ) {
		printf( "you guessed blah correctly (static)" );
	} else {
		printf( "you're wrong! (static)" );
	}

	// VS2005 Release should generate:
	// cmp	esi, -1146709846			; bba698aaH
	if ( hash == dynamic_murmurhash( "blah", 0 ) ) {
		printf( "you guessed blah correctly (dynamic)" );
	} else {
		printf( "you're wrong! (dynamic)" );
	}
}