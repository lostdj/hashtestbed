/*

	Unless specified otherwise, this file and the repository it's in are:

		hashtestbed - https://github.com/lostdj/hashtestbed
		Written in 2018 by Timofey Lagutin (lostdj at gmail)
		To the extent possible under law, the author(s) have dedicated all copyright
		and related and neighboring rights to this software to the public domain worldwide.
		This software is distributed without any warranty.
		You should have received a copy of the CC0 Public Domain Dedication along with this software.
		If not, see:
			http://creativecommons.org/publicdomain/zero/1.0/

*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include <iostream>
#include <iomanip>
#include <type_traits>

#include <immintrin.h>

//
namespace
meow
{

#include "meow_hash-0.1a/meow_hash.h"

} // namespace meow

//
namespace
murmur3
{

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER) && (_MSC_VER < 1600)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

// Other compilers

#else	// defined(_MSC_VER)

//#include <stdint.h>

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------

//void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );

//void MurmurHash3_x86_128 ( const void * key, int len, uint32_t seed, void * out );

//void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );

//-----------------------------------------------------------------------------

#endif // _MURMURHASH3_H_

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

//#include "MurmurHash3.h"

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER)

#define FORCE_INLINE	__forceinline

#include <stdlib.h>

#define ROTL32(x,y)	_rotl(x,y)
#define ROTL64(x,y)	_rotl64(x,y)

#define BIG_CONSTANT(x) (x)

// Other compilers

#else	// defined(_MSC_VER)

#define	FORCE_INLINE inline __attribute__((always_inline))

inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}

inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

FORCE_INLINE uint32_t getblock32 ( const uint32_t * p, int i )
{
  return p[i];
}

FORCE_INLINE uint64_t getblock64 ( const uint64_t * p, int i )
{
  return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

FORCE_INLINE uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

//----------

FORCE_INLINE uint64_t fmix64 ( uint64_t k )
{
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}

//-----------------------------------------------------------------------------

static void MurmurHash3_x86_32 ( const void * key, int len,
                          uint32_t seed, void * out )
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 4;

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

  for(int i = -nblocks; i; i++)
  {
    uint32_t k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = ROTL32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;

  switch(len & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  *(uint32_t*)out = h1;
}

//-----------------------------------------------------------------------------

//static void MurmurHash3_x86_128 ( const void * key, const int len,
//                           uint32_t seed, void * out )
//{
//  const uint8_t * data = (const uint8_t*)key;
//  const int nblocks = len / 16;

//  uint32_t h1 = seed;
//  uint32_t h2 = seed;
//  uint32_t h3 = seed;
//  uint32_t h4 = seed;

//  const uint32_t c1 = 0x239b961b;
//  const uint32_t c2 = 0xab0e9789;
//  const uint32_t c3 = 0x38b34ae5;
//  const uint32_t c4 = 0xa1e38b93;

//  //----------
//  // body

//  const uint32_t * blocks = (const uint32_t *)(data + nblocks*16);

//  for(int i = -nblocks; i; i++)
//  {
//    uint32_t k1 = getblock32(blocks,i*4+0);
//    uint32_t k2 = getblock32(blocks,i*4+1);
//    uint32_t k3 = getblock32(blocks,i*4+2);
//    uint32_t k4 = getblock32(blocks,i*4+3);

//    k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;

//    h1 = ROTL32(h1,19); h1 += h2; h1 = h1*5+0x561ccd1b;

//    k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;

//    h2 = ROTL32(h2,17); h2 += h3; h2 = h2*5+0x0bcaa747;

//    k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;

//    h3 = ROTL32(h3,15); h3 += h4; h3 = h3*5+0x96cd1c35;

//    k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;

//    h4 = ROTL32(h4,13); h4 += h1; h4 = h4*5+0x32ac3b17;
//  }

//  //----------
//  // tail

//  const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

//  uint32_t k1 = 0;
//  uint32_t k2 = 0;
//  uint32_t k3 = 0;
//  uint32_t k4 = 0;

//  switch(len & 15)
//  {
//  case 15: k4 ^= tail[14] << 16;
//  case 14: k4 ^= tail[13] << 8;
//  case 13: k4 ^= tail[12] << 0;
//           k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;

//  case 12: k3 ^= tail[11] << 24; [[clang::fallthrough]];
//  case 11: k3 ^= tail[10] << 16;
//  case 10: k3 ^= tail[ 9] << 8;
//  case  9: k3 ^= tail[ 8] << 0;
//           k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;

//  case  8: k2 ^= tail[ 7] << 24;
//  case  7: k2 ^= tail[ 6] << 16;
//  case  6: k2 ^= tail[ 5] << 8;
//  case  5: k2 ^= tail[ 4] << 0;
//           k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;

//  case  4: k1 ^= tail[ 3] << 24;
//  case  3: k1 ^= tail[ 2] << 16;
//  case  2: k1 ^= tail[ 1] << 8;
//  case  1: k1 ^= tail[ 0] << 0;
//           k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
//  };

//  //----------
//  // finalization

//  h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;

//  h1 += h2; h1 += h3; h1 += h4;
//  h2 += h1; h3 += h1; h4 += h1;

//  h1 = fmix32(h1);
//  h2 = fmix32(h2);
//  h3 = fmix32(h3);
//  h4 = fmix32(h4);

//  h1 += h2; h1 += h3; h1 += h4;
//  h2 += h1; h3 += h1; h4 += h1;

//  ((uint32_t*)out)[0] = h1;
//  ((uint32_t*)out)[1] = h2;
//  ((uint32_t*)out)[2] = h3;
//  ((uint32_t*)out)[3] = h4;
//}

//-----------------------------------------------------------------------------

//static void MurmurHash3_x64_128 ( const void * key, const int len,
//                           const uint32_t seed, void * out )
//{
//  const uint8_t * data = (const uint8_t*)key;
//  const int nblocks = len / 16;

//  uint64_t h1 = seed;
//  uint64_t h2 = seed;

//  const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
//  const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

//  //----------
//  // body

//  const uint64_t * blocks = (const uint64_t *)(data);

//  for(int i = 0; i < nblocks; i++)
//  {
//    uint64_t k1 = getblock64(blocks,i*2+0);
//    uint64_t k2 = getblock64(blocks,i*2+1);

//    k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

//    h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

//    k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

//    h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
//  }

//  //----------
//  // tail

//  const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

//  uint64_t k1 = 0;
//  uint64_t k2 = 0;

//  switch(len & 15)
//  {
//  case 15: k2 ^= ((uint64_t)tail[14]) << 48;
//  case 14: k2 ^= ((uint64_t)tail[13]) << 40;
//  case 13: k2 ^= ((uint64_t)tail[12]) << 32;
//  case 12: k2 ^= ((uint64_t)tail[11]) << 24;
//  case 11: k2 ^= ((uint64_t)tail[10]) << 16;
//  case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
//  case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;
//           k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

//  case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;
//  case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
//  case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
//  case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
//  case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
//  case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
//  case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
//  case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
//           k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
//  };

//  //----------
//  // finalization

//  h1 ^= len; h2 ^= len;

//  h1 += h2;
//  h2 += h1;

//  h1 = fmix64(h1);
//  h2 = fmix64(h2);

//  h1 += h2;
//  h2 += h1;

//  ((uint64_t*)out)[0] = h1;
//  ((uint64_t*)out)[1] = h2;
//}

//-----------------------------------------------------------------------------

} // namespace murmur3

//
namespace
xxhash
{
#define XXH_STATIC_LINKING_ONLY
//#define XXH_INLINE_ALL

#include "xxHash/xxhash.h"

#include "xxHash/xxhash.c"

} // namespace xxhash

//
namespace
t1ha
{

#define T1HA0_AESNI_AVAILABLE 0

#include "t1ha/t1ha.h"

//#include "t1ha/src/t1ha0_ia32aes_avx.c"
//#include "t1ha/src/t1ha0_ia32aes_avx2.c"
#include "t1ha/src/t1ha0_ia32aes_noavx.c"
#include "t1ha/src/t1ha1.c"
#include "t1ha/src/t1ha2.c"

} // namespace t1ha

//
namespace
xua
{

//
#define elif else if

#define ceif  if constexpr
#define celif elif constexpr
#define celse else

#define cexpr constexpr

#ifndef null
	#define null nullptr
#endif

//
#define UNUSED(x__) ((void)x__)

//
using b1 = unsigned char;
using byte = unsigned char;
using octet = unsigned char;
using s1 = signed char;
using u1 = unsigned char;
using s2 = signed short int;
using u2 = unsigned short int;
using s4 = signed int;
using u4 = unsigned int;
using s8 = signed long int;
using u8 = unsigned long int;
using sw = s8;
using uw = u8;
cexpr inline const auto wszbytes = sizeof(uw);
cexpr inline const auto wszbits = wszbytes * 8;
using f4 = float;
using f8 = double;

static_assert(sizeof(s1) == 1);
static_assert(sizeof(s2) == 2);
static_assert(sizeof(s4) == 4);
static_assert(sizeof(s8) == 8);
static_assert(wszbytes == 8);
static_assert(sizeof(f4) == 4);
static_assert(sizeof(f8) == 8);

//
template<typename R, typename T>
cexpr inline
R
rc(T t)
{
	return reinterpret_cast<R>(t);
}

// https://stackoverflow.com/questions/24398102/constexpr-and-initialization-of-a-static-const-void-pointer-with-reinterpret-cas
template<typename R, typename T>
cexpr inline
R
rcce(T t)
{
	return __builtin_constant_p(rc<R>(t)) ? rc<R>(t) : rc<R>(t);
}

template <typename T, uw N>
cexpr inline
uw
arrlen(T(&)[N])
{
	return N;
}

//
template<typename T>
cexpr inline
T
pad(T base, uw alignment)
{
	return T(((uw)base + (alignment - 1)) & ~(alignment - 1));
}

template<typename T>
cexpr inline
T
pad2(T base)
{
	return T(pad(base, 2));
}

template<typename T>
cexpr inline
T
pad4(T base)
{
	return T(pad(base, 4));
}

template<typename T>
cexpr inline
T
pad8(T base)
{
	return T(pad(base, 8));
}

template<typename T>
cexpr inline
T
padw(T base)
{
	return T(pad(base, sizeof(uw)));
}

template<typename T>
cexpr inline
bool
aligned(T base, uw alignment)
{
	return base == pad(base, alignment);
}

template<typename T>
cexpr inline
bool
aligned2(T base)
{
	return base == pad2(base);
}

template<typename T>
cexpr inline
bool
aligned4(T base)
{
	return base == pad4(base);
}

template<typename T>
cexpr inline
bool
aligned8(T base)
{
	return base == pad8(base);
}

template<typename T>
cexpr inline
bool
alignedw(T base)
{
	return base == padw(base);
}

//
template<typename T>
constexpr inline
auto
abs(T v)
{
	return v < 0 ? -v : v;
}

template<typename T>
cexpr inline
auto
min(T a, T b)
{
    return (b < a) ? b : a;
}

template<typename T>
cexpr inline
auto
max(T a, T b)
{
    return (a < b) ? b : a;
}

//
template<typename A, typename B> inline
void
mcopy(const A src, B dst, uw bytes)
{
	static_assert(std::is_pointer<A>::value);
	static_assert(std::is_pointer<B>::value);

	memcpy((void*)dst, (const void*)src, bytes);
}

//
inline
int
soutflush()
{
	return fflush(stdout);
}

inline
int
serrflush()
{
	return fflush(stderr);
}

template<typename... Args>
inline
int
sout(const char* fmt, Args&&... args)
{
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wformat-security"
	#pragma clang diagnostic ignored "-Wformat-nonliteral"
	int r = fprintf(stdout, fmt, args...);
	soutflush();
	#pragma clang diagnostic pop

	return r;
}

template<typename... Args>
inline
int
serr(const char* fmt, Args&&... args)
{
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wformat-security"
	#pragma clang diagnostic ignored "-Wformat-nonliteral"
	int r = fprintf(stderr, fmt, args...);
	serrflush();
	#pragma clang diagnostic pop

	return r;
}

} // namespace xua

//
using namespace xua;

//
namespace
fubar
{

cexpr inline
u8
fubar(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	u8 d = length;

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		u8 tmp = 0;

		if(padded8 && remain > 7)
			tmp = *rcce<const u8*>(dat);
		else
			mcopy(dat, &tmp, remain);

		d += tmp;
	}

	return d;
}

} // namespace fubar

//
namespace
kr
{

cexpr inline
u8
kr(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	u8 h = 0;

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		u8 tmp = 0;

		if(padded8 && remain > 7)
			tmp = *rcce<const u8*>(dat);
		else
			mcopy(dat, &tmp, remain);

		h = 31 * h + tmp;
	}

	return h;
}

cexpr inline
u8
krl1(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	u8 h = length;

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		u8 tmp = 0;

		if(padded8 && remain > 7)
			tmp = *rcce<const u8*>(dat);
		else
			mcopy(dat, &tmp, remain);

		h = 31 * h + tmp;
	}

	return h;
}

cexpr inline
u8
krl2(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	u8 h = 0;

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		u8 tmp = 0;

		if(padded8 && remain > 7)
			tmp = *rcce<const u8*>(dat);
		else
			mcopy(dat, &tmp, remain);

		h = 31 * h + tmp;
	}

	return 31 * h + length;
}

} // namespace kr

//
namespace
murmur3our
{

static
cexpr inline
u4
murmur3our(const void* key, int len, u4 seed)
{
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wunused-local-typedef"

	typedef u1 uint8_t;
	typedef s1 int8_t;
	typedef u2 uint16_t;
	typedef s2 int16_t;
	typedef u4 uint32_t;
	typedef s4 int32_t;
	typedef u8 uint64_t;
	typedef s8 int64_t;

	#pragma clang diagnostic pop

	struct
	halp
	{
		static inline
		uint32_t rotl32 ( uint32_t x, int8_t r )
		{
			return (x << r) | (x >> (32 - r));
		}

		// Block read - if your platform needs to do endian-swapping or can only
		// handle aligned reads, do the conversion here.
		static inline
		uint32_t getblock32 ( const uint32_t * p, int i )
		{
			return p[i];
		}

		// Finalization mix - force all bits of a hash block to avalanche.
		static inline
		uint32_t fmix32 ( uint32_t h )
		{
			h ^= h >> 16;
			h *= 0x85ebca6b;
			h ^= h >> 13;
			h *= 0xc2b2ae35;
			h ^= h >> 16;

			return h;
		}
	};

	const uint8_t * data = (const uint8_t*)key;
	const int nblocks = len / 4;

	uint32_t h1 = seed;

	const uint32_t c1 = 0xcc9e2d51;
	const uint32_t c2 = 0x1b873593;

	//----------
	// body

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wcast-align"
	const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);
	#pragma clang diagnostic pop

	for(int i = -nblocks; i; i++)
	{
		uint32_t k1 = halp::getblock32(blocks,i);

		k1 *= c1;
		k1 = halp::rotl32(k1,15);
		k1 *= c2;

		h1 ^= k1;
		h1 = halp::rotl32(h1,13);
		h1 = h1*5+0xe6546b64;
	}

	//----------
	// tail

	const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

	uint32_t k1 = 0;

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wsign-conversion"

	switch(len & 3)
	{
		case 3: k1 ^= tail[2] << 16;  [[clang::fallthrough]];
		case 2: k1 ^= tail[1] << 8;   [[clang::fallthrough]];
		case 1: k1 ^= tail[0];
					k1 *= c1; k1 = halp::rotl32(k1,15); k1 *= c2; h1 ^= k1;
	};

	//----------
	// finalization

	h1 ^= len;

	#pragma clang diagnostic pop

	h1 = halp::fmix32(h1);

//  *(uint32_t*)out = h1;
	return h1;
}

} // namespace murmur3our

//
namespace
pcg
{

struct
Pcg
{
	// https://github.com/imneme/pcg-c-basic/
	// bc39cd76ac3d541e618606bcc6e1e5ba5e5e6aa3 @ Oct 18, 2014

	// Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
	//
	// Licensed under the Apache License, Version 2.0.

	u8 state;
	u8 inc;

	cexpr inline
	Pcg(u8 state, u8 inc = 0) : state(state), inc((inc << 1u) | 1u)
	{
		u8 init_state = this->state;
		this->state = 0u;
		advance();
		this->state += init_state;
		advance();
	}

	// XSH RR
	cexpr inline
	u4
	next4_base(u8 oldstate)
	{
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wshorten-64-to-32"
    u4 xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
		#pragma clang diagnostic pop
    u4 rot = oldstate >> 59u;

    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
	}

	cexpr inline
	u8
	advance()
	{
		u8 oldstate = this->state;
		this->state = oldstate * 6364136223846793005ULL + inc;

		return oldstate;
	}

	cexpr inline
	u4
	next4()
	{
		return next4_base(advance());
	}

	cexpr inline
	u8
	next8()
	{
		// From https://github.com/imneme/pcg-c-basic/blob/master/pcg32x2-demo.c
		/*
		 * This code shows how you can cope if you're on a 32-bit platform (or a
		 * 64-bit platform with a mediocre compiler) that doesn't support 128-bit math,
		 * or if you're using the basic version of the library which only provides
		 * 32-bit generation.
		 *
		 * Here we build a 64-bit generator by tying together two 32-bit generators.
		 * Note that we can do this because we set up the generators so that each
		 * 32-bit generator has a *totally different* different output sequence
		 * -- if you tied together two identical generators, that wouldn't be nearly
		 * as good.
		 *
		 * For simplicity, we keep the period fixed at 2^64.  The state space is
		 * approximately 2^254 (actually  2^64 * 2^64 * 2^63 * (2^63 - 1)), which
		 * is huge.
		*/
		// And
		// "The stream for each of the two generators *must* be distinct"
		// But, well...
		// Cheating here.
		// I can imagine it can be bad.

		auto a1 = advance();
		auto a2 = advance();

		auto n1 = next4_base(a1);
		auto n2 = next4_base(a2);

		u8 r1 = n1;
		u8 r2 = n2;

		return (r1 << 32) | r2;
	}

	// Generate a uniformly distributed number, r, where 0 <= r < bound.
	cexpr inline
	u4
	next4_v1(u4 bound)
	{
		// To avoid bias, we need to make the range of the RNG a multiple of
		// bound, which we do by dropping output less than a threshold.
		// A naive scheme to calculate the threshold would be to do
		//
		//     uint32_t threshold = 0x100000000ull % bound;
		//
		// but 64-bit div/mod is slower than 32-bit div/mod (especially on
		// 32-bit platforms).  In essence, we do
		//
		//     uint32_t threshold = (0x100000000ull-bound) % bound;
		//
		// because this version will calculate the same modulus, but the LHS
		// value is less than 2^32.

		uint32_t threshold = -bound % bound;

		// Uniformity guarantees that this loop will terminate.  In practice, it
		// should usually terminate quickly; on average (assuming all bounds are
		// equally likely), 82.25% of the time, we can expect it to require just
		// one iteration.  In the worst case, someone passes a bound of 2^31 + 1
		// (i.e., 2147483649), which invalidates almost 50% of the range.  In
		// practice, bounds are typically small and only a tiny amount of the range
		// is eliminated.
		for(;;)
		{
			u4 r = next4();
			if(r >= threshold)
				return r % bound;
		}
	}

	// http://www.pcg-random.org/posts/bounded-rands.html
	cexpr inline
	u4
	next4_v2(u4 bound)
	{
		u4 x = next4();
		u8 m = u8(x) * u8(bound);
		u4 l = u4(m);
		if(l < bound)
		{
			u4 t = -bound;
			if(t >= bound)
			{
				t -= bound;
				if(t >= bound)
					t %= bound;
			}

			while(l < t)
			{
				x = next4();
				m = u8(x) * u8(bound);
				l = u4(m);
			}
		}

		return m >> 32;
	}
}; // Pcg

cexpr inline
u4
pcg4(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	Pcg pcg(length);
//		u8 s = Pcg(length).next8();

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 d = 0;

		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		if(padded8 && remain > 7)
			d = *rcce<const u8*>(dat);
		else
			mcopy(dat, &d, remain);

		pcg.state += d;
//			Pcg pcg(d ^ s, 0);
// //			pcg.inc = s;
		pcg.advance();
//			s = pcg.next8();
	}

	return pcg.next4_base(pcg.state);
//		return Pcg(s).next4();
}

cexpr inline
u4
pcg4v2(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	Pcg pcg(length);

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 d = 0;

		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		if(padded8 && remain > 7)
			d = *rcce<const u8*>(dat);
		else
			mcopy(dat, &d, remain);

		pcg.state += d;
		pcg.advance();
		pcg.advance();
	}

	return pcg.next4_base(pcg.state);
}

cexpr inline
u4
pcg4v3(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	Pcg pcg(0ul, length);

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 d = 0;

		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		if(padded8 && remain > 7)
			d = *rcce<const u8*>(dat);
		else
			mcopy(dat, &d, remain);

		pcg.state += d;
		pcg.advance();
	}

	return pcg.next4_base(pcg.state);
}

cexpr inline
u4
pcg4S(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	Pcg pcg(0ul);

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 d = 0;

		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		if(padded8 && remain > 7)
			d = *rcce<const u8*>(dat);
		else
			mcopy(dat, &d, remain);

		pcg.state += d;
		pcg.advance();
	}

	Pcg r(length);
	r.state += pcg.state;

	return r.next4();
}

cexpr inline
u4
pcg4b1(const void* data, const u8 length, const u4 b)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	Pcg pcg(length);

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 d = 0;

		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		if(padded8 && remain > 7)
			d = *rcce<const u8*>(dat);
		else
			mcopy(dat, &d, remain);

		pcg.state += d;
		pcg.advance();
	}

	return pcg.next4_v1(b);
}

cexpr inline
u4
pcg4b2(const void* data, const u8 length, const u4 b)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	Pcg pcg(length);

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 d = 0;

		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		if(padded8 && remain > 7)
			d = *rcce<const u8*>(dat);
		else
			mcopy(dat, &d, remain);

		pcg.state += d;
		pcg.advance();
	}

	return pcg.next4_v2(b);
}

// I'd imagine it's prone to some attacks.
cexpr inline
u4
pcg4z1(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	Pcg pcg(length);
	u8 z = 0;

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 d = 0;

		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		if(padded8 && remain > 7)
			d = *rcce<const u8*>(dat);
		else
			mcopy(dat, &d, remain);

		pcg.state += d;
		pcg.advance();

		z += abs(__builtin_clz(d) - ((8 - remain) * 8));
	}

	pcg.state += z;
	pcg.advance();

	return pcg.next4_base(pcg.state);
}

// I'd imagine it's prone to some attacks.
cexpr inline
u4
pcg4z2(const void* data, const u8 length)
{
	const byte* dat = (const byte*)data;

	const auto padded8 = aligned8(dat);

	Pcg pcg(0ul);
	u8 z = 0;

	for(uw i = 0; i < length; dat += 8, i += 8)
	{
		u8 d = 0;

		u8 remain = length - i;
		remain = remain > 8 ? 8 : remain;

		if(padded8 && remain > 7)
			d = *rcce<const u8*>(dat);
		else
			mcopy(dat, &d, remain);

		pcg.state += d;
		pcg.advance();

		z += abs(__builtin_clz(d) - ((8 - remain) * 8));
	}

	pcg.state += z;
	pcg.advance();

	Pcg r(length);
	r.state += pcg.state;

	return r.next4();
}

} // namespace pcg

//
struct
HashTestbed
{
	u8 table_size = 0;
	u4* table = null;
	u8 total_iters = 0;
	cexpr static u8 collision_map_size = 536870912; // 512mib.
	byte* collision_map = null; // TODO: Currently it handles only 4 bytes, thus the map is 512mib.
	u8 collisions = 0;

	// Use me in your tests.
	u8 iter = 0;
	byte buf[4096] = {0};

	// Return me.
	struct
	Res
	{
		byte* data = null;
		u8 length = 4;
	};

	void
	print_head(const char* name)
	{
		sout("#### %s\n", name);
		sout("Table size: %llu  \n", table_size);
		sout("Iterations: %llu  \n", total_iters);
		sout("Perfect score: %llu  \n", total_iters < table_size ? (100 * total_iters) : (100 * table_size));

		sout("\n");

		sout("%s",
R"(
|      | score | altscore | collisions | ok/% zero/% above/% below/% | <% <=25/% <=50/% <=75/% <=99/% | >% >100/% >=125/% >=150/% >=175/% >=200/% |
|-----:|------:|---------:|-----------:|----------------------------:|-------------------------------:|------------------------------------------:|
)"
		);
	}

	auto
	print_entry(const char* name)
	{
		//
		const double perentry = ::ceil((double)total_iters / table_size);
		//std::cout << "perentry: " << perentry << std::endl;

		//
		double score = 0;
		double altscore = 0;

		u8 ok_total = 0;
		u8 zero_total = 0;
		u8 over_total = 0;
		u8 below_total = 0;

		u8 stats[] =
		{
			0, // 25   // 0
			0, // 50   // 1
			0, // 75   // 2
			0, // <100 // 3
			0, // >100 // 4
			0, // 125  // 5
			0, // 150  // 6
			0, // 175  // 7
			0, // 200  // 8
		};

		//
		for(u8 i = 0; i < table_size; i++)
		{
			const u4 n = table[i];
			const double p = (double)n / perentry * 100;

			if(n == perentry)
				++ok_total;
			elif(n == 0)
				++zero_total, ++below_total;
			elif(n > perentry)
			{
				++over_total;

				++stats[(uw)::floor( ::min(200., p) / 200 * (arrlen(stats) - 1) )];
			}
			elif(n < perentry)
			{
				++below_total;

				++stats[(uw)::floor( (::min(99., p) - 1) / 200 * (arrlen(stats) - 1) )];
			}

			//std::cout << "p: " << p << " | " << (p > 100 ? 100 - p : p) << std::endl;

			score += p > 100 ? 100 - p : p;

			// The theoretical metric from "Red Dragon Book".
//			altscore += n * (n + 1);
//			altscore += n * (n + 1) / 2;
//			altscore += n * (n);
//			altscore += (n * (n + 1) / 2) / (((double)total_iters / (2*table_size)) * ((double)total_iters + (2*table_size) - 1));
//			altscore += (n * (n + 1)) / (((double)total_iters / (2*table_size)) * ((double)total_iters + (2*table_size) - 1));
			altscore += ((double)n * (n + 1)) / (((double)total_iters / table_size) * ((double)total_iters + table_size ));
//			altscore += ((double)n * (n + 1)) / (((double)total_iters / table_size) * ((double)total_iters + table_size + 1));

//			for(int k = 0; k < pperentry; k++)
//				serr("|");
//			serr("\n");
		}

//		auto pre = "<pre>";
//		auto cpre = "</pre>";
		auto pre = "";
		auto cpre = "";
		auto sepa = u8" ▪ ";
//		sout("| %s | %llu | derp |\n", name, points);
		/*<< std::setfill(' ') << std::setw(5) */
		std::cout.fill(' ');
		std::cout.precision(2);
		std::cout << "| "
		          << name << " | "
		          <<pre<< std::fixed<< score <<cpre<< " | "
//		          <<pre<< std::fixed<< (altscore * table_size / ((double)total_iters * (total_iters + 2 * table_size - 1))) <<cpre<< " | "
//		          <<pre<< std::fixed<< (altscore / (((double)total_iters / (2*table_size)) * (total_iters + 2 * (2*table_size) - 1))) <<cpre<< " | "
//              <<pre<< std::fixed<< (altscore / (((double)total_iters / table_size) * (total_iters + 2 * table_size - 1))) <<cpre<< " | "
//              <<pre<< std::fixed<< (altscore / (((double)total_iters / (2*table_size)) * (total_iters + (2*table_size) - 1))) <<cpre<< " | "
//		          <<pre<< std::fixed<< (altscore / (((double)total_iters / table_size) * (total_iters + 2 * table_size - 1))) <<cpre<< " | "
		            <<pre<< std::fixed<< altscore <<cpre<< " | "
		            <<pre<< collisions <<cpre<< " | "
							  <<pre
		            << std::setw(7)<<ok_total    <<"/"<< /*std::setw(7)<<*/((double)ok_total/table_size*100) <<sepa
		            << std::setw(7)<<zero_total  <<"/"<< /*std::setw(7)<<*/((double)zero_total/table_size*100) <<sepa
		            << std::setw(7)<<over_total  <<"/"<< /*std::setw(7)<<*/((double)over_total/table_size*100) <<sepa
		            << std::setw(7)<<below_total <<"/"<< /*std::setw(7)<<*/((double)below_total/table_size*100)
		            <<cpre<< " | "
		          <<""
		            <<pre
		            << std::setw(7)<<stats[0]<<"/"<< /*std::setw(7)<<*/((double)stats[0]/table_size*100) <<sepa
		            << std::setw(7)<<stats[1]<<"/"<< /*std::setw(7)<<*/((double)stats[1]/table_size*100) <<sepa
		            << std::setw(7)<<stats[2]<<"/"<< /*std::setw(7)<<*/((double)stats[2]/table_size*100) <<sepa
		            << std::setw(7)<<stats[3]<<"/"<< /*std::setw(7)<<*/((double)stats[4]/table_size*100)
		            <<cpre<< " | "
		           <<""
		             <<pre
		             << std::setw(7)<<stats[4]<<"/"<< /*std::setw(7)<<*/((double)stats[4]/table_size*100) <<sepa
		             << std::setw(7)<<stats[5]<<"/"<< /*std::setw(7)<<*/((double)stats[5]/table_size*100) <<sepa
		             << std::setw(7)<<stats[6]<<"/"<< /*std::setw(7)<<*/((double)stats[6]/table_size*100) <<sepa
		             << std::setw(7)<<stats[7]<<"/"<< /*std::setw(7)<<*/((double)stats[7]/table_size*100) <<sepa
		             << std::setw(7)<<stats[8]<<"/"<< /*std::setw(7)<<*/((double)stats[8]/table_size*100)
		             <<cpre<< " | "

		          << std::endl;
	}

	void
	print_end()
	{
		sout("\n");
	}

	void
	iter_reset()
	{
		if(table)
			delete[] table;
		table = new u4[table_size]();

		if(collision_map)
			if(munmap(collision_map, collision_map_size))
				perror("Error unmapping collision_map."),
				exit(EXIT_FAILURE);

		collision_map = (byte*)mmap(0, collision_map_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if(collision_map == MAP_FAILED)
			serr("Error mmaping collision_map"),
			exit(EXIT_FAILURE);

//		if(!collision_map)
//			collision_map = new byte[collision_map_size];

//		memset(collision_map, 0, collision_map_size);

		// ^ Both are equally slow.

		collisions = 0;

		iter = 0;
	}

	void
	hash_collides(u8 hash)
	{
		u4 h = (u4)hash;
		u8 i = h >> 3;
		byte c = collision_map[i];
		if((c >> (h & 0b111)) & 0b1)
			++collisions
//		  ,serr("![-- Collision @ iter %ul --]!", iter);
			;

		c = c | (1 << (h & 0b111));
		collision_map[i] = c;
	}

	void
	hash(u4 hash)
	{
		table[hash % table_size]++;

		hash_collides(hash);
	}

	void
	hash(u8 hash)
	{
		table[hash % table_size]++;

		hash_collides(hash);
	}

	u4
	hashx_h(u4 hash)
	{
//		auto z = __builtin_clz(table_size);
//		auto z = 32 - __builtin_clz(table_size);
//
//		return hash ^ (hash >> z);

		if(table_size < 0xFFFF)
			hash = hash ^ (hash >> 16);

		return hash;
	}

	void
	hashx(u4 hash)
	{
		table[hashx_h(hash) % table_size]++;

		hash_collides(hash);
	}

	void
	hashx(u8 hash)
	{
		hash = hash ^ (hash >> 32);

		table[hashx_h(hash) % table_size]++;

		hash_collides(hash);
	}

	template<typename Ftest, typename Fhash>
	void
	test(const char* tname, const char* hname, HashTestbed& tbs, Ftest&& ftest, Fhash&& fhash)
	{
		serr("\n");serr("*** *** %s: %s\n", tname, hname);
		iter_reset();
		for(tbs.iter = 0; tbs.iter < tbs.total_iters; tbs.iter++)
		{
			auto r = ftest(tbs);
			tbs.hash(fhash(r));
		}
		tbs.print_entry(hname);
		serr("-------------------------------------------------------------------------------\n");
	}

	template<typename Ftest, typename Fhash>
	void
	testx(const char* tname, const char* hname, HashTestbed& tbs, Ftest&& ftest, Fhash&& fhash)
	{
		serr("\n");serr("*** *** %s: %s\n", tname, hname);
		iter_reset();
		for(tbs.iter = 0; tbs.iter < tbs.total_iters; tbs.iter++)
		{
			auto r = ftest(tbs);
			tbs.hashx(fhash(r));
		}
		tbs.print_entry(hname);
		serr("-------------------------------------------------------------------------------\n");
	}

	template<typename F>
	void
	test(const char* name, const u8 table_size, const u8 iters, F&& f, bool dohash = true)
	{
		//
		HashTestbed& tbs = *this;

		//
		tbs.table_size = table_size;
		tbs.total_iters = iters;

		//
		serr("\n");serr("*** '%s', table_size: %llu, iters: %llu\n", name, table_size, iters);
		tbs.print_head(name);

		//
		if(!dohash)
			goto skiphash;

		;
		test(name, "fubar", tbs, f, [](Res& r){return fubar::fubar(r.data, r.length);});
		testx(name, "^ X", tbs, f, [](Res& r){return fubar::fubar(r.data, r.length);});
		test(name, "kr", tbs, f, [](Res& r){return kr::kr(r.data, r.length);});
		testx(name, "^ X", tbs, f, [](Res& r){return kr::kr(r.data, r.length);});
		test(name, "krl1", tbs, f, [](Res& r){return kr::krl1(r.data, r.length);});
		test(name, "krl2", tbs, f, [](Res& r){return kr::krl2(r.data, r.length);});
		testx(name, "^ X", tbs, f, [](Res& r){return kr::krl2(r.data, r.length);});
		test(name, "pcg84v1", tbs, f, [](Res& r){return (u4)pcg::pcg4(r.data, r.length);});
		testx(name, "^ X", tbs, f, [](Res& r){return (u4)pcg::pcg4(r.data, r.length);});
		test(name, "pcg84v2", tbs, f, [](Res& r){return (u4)pcg::pcg4v2(r.data, r.length);});
		test(name, "pcg84v3", tbs, f, [](Res& r){return (u4)pcg::pcg4v3(r.data, r.length);});
		test(name, "pcg84S", tbs, f, [](Res& r){return (u4)pcg::pcg4S(r.data, r.length);});
		testx(name, "^ X", tbs, f, [](Res& r){return (u4)pcg::pcg4S(r.data, r.length);});
		test(name, "pcg84b1", tbs, f, [&](Res& r){return (u4)pcg::pcg4b1(r.data, r.length, table_size);});
		test(name, "pcg84b2", tbs, f, [&](Res& r){return (u4)pcg::pcg4b2(r.data, r.length, table_size);});
		test(name, "pcg84z1", tbs, f, [&](Res& r){return (u4)pcg::pcg4z1(r.data, r.length);});
		test(name, "pcg84z2", tbs, f, [&](Res& r){return (u4)pcg::pcg4z2(r.data, r.length);});
		test(name, "meow0.1a-18", tbs, f, [](Res& r){return (u8)(meow::MeowHash1(0, r.length, r.data).Sub[0]);});
		testx(name, "^ X", tbs, f, [](Res& r){return (u8)(meow::MeowHash1(0, r.length, r.data).Sub[0]);});
		test(name, "meow0.1a-184hi", tbs, f, [](Res& r){return (u4)(meow::MeowHash1(0, r.length, r.data).Sub[0] >> 32);});
		test(name, "meow0.1a-184lo", tbs, f, [](Res& r){return (u4)(meow::MeowHash1(0, r.length, r.data).Sub[0]);});
		testx(name, "^ X", tbs, f, [](Res& r){return (u4)(meow::MeowHash1(0, r.length, r.data).Sub[0]);});
//		test(name, "meow24", tbs, f, [](Res& r){return (u4)(meow::MeowHash2(0, r.length, r.data).Sub[0]);});
//		test(name, "meow28", tbs, f, [](Res& r){return (u8)(meow::MeowHash2(0, r.length, r.data).Sub[0]);});
//		test(name, "meow44", tbs, f, [](Res& r){return (u4)(meow::MeowHash4(0, r.length, r.data).Sub[0]);});
//		test(name, "meow48", tbs, f, [](Res& r){return (u8)(meow::MeowHash4(0, r.length, r.data).Sub[0]);});
//		test(name, "murmur34our", tbs, f, [](Res& r){return murmur3our::murmur3our(r.data, r.length, 0);});
		test(name, "murmur34", tbs, f, [](Res& r){u4 out = 0; murmur3::MurmurHash3_x86_32(r.data, r.length, 0, &out); return out;});
		testx(name, "^ X", tbs, f, [](Res& r){u4 out = 0; murmur3::MurmurHash3_x86_32(r.data, r.length, 0, &out); return out;});
		test(name, "xxHash4", tbs, f, [](Res& r){return (u4)xxhash::XXH32(r.data, r.length, 0);});
		testx(name, "^ X", tbs, f, [](Res& r){return (u4)xxhash::XXH32(r.data, r.length, 0);});
		test(name, "xxHash8", tbs, f, [](Res& r){return (u8)(xxhash::XXH64(r.data, r.length, 0));});
		testx(name, "^ X", tbs, f, [](Res& r){return (u8)(xxhash::XXH64(r.data, r.length, 0));});
		test(name, "xxHash84hi", tbs, f, [](Res& r){return (u4)(xxhash::XXH64(r.data, r.length, 0) >> 32);});
		test(name, "xxHash84lo", tbs, f, [](Res& r){return (u4)(xxhash::XXH64(r.data, r.length, 0));});
		test(name, "t1ha28", tbs, f, [](Res& r){return (u8)(t1ha::t1ha2_atonce(r.data, r.length, 0));});
		testx(name, "^ X", tbs, f, [](Res& r){return (u8)(t1ha::t1ha2_atonce(r.data, r.length, 0));});
		test(name, "t1ha284hi", tbs, f, [](Res& r){return (u4)(t1ha::t1ha2_atonce(r.data, r.length, 0) >> 32);});
		test(name, "t1ha284lo", tbs, f, [](Res& r){return (u4)(t1ha::t1ha2_atonce(r.data, r.length, 0));});
		testx(name, "^ X", tbs, f, [](Res& r){return (u4)(t1ha::t1ha2_atonce(r.data, r.length, 0));});
		test(name, "t1ha18", tbs, f, [](Res& r){return (u8)(t1ha::t1ha1_le(r.data, r.length, 0));});
		test(name, "t1ha184hi", tbs, f, [](Res& r){return (u4)(t1ha::t1ha1_le(r.data, r.length, 0) >> 32);});
		test(name, "t1ha184lo", tbs, f, [](Res& r){return (u4)(t1ha::t1ha1_le(r.data, r.length, 0));});
		test(name, "t1ha08", tbs, f, [](Res& r){return (u8)(t1ha::t1ha0(r.data, r.length, 0));});
		test(name, "t1ha084hi", tbs, f, [](Res& r){return (u4)(t1ha::t1ha0(r.data, r.length, 0) >> 32);});
		test(name, "t1ha084lo", tbs, f, [](Res& r){return (u4)(t1ha::t1ha0(r.data, r.length, 0));});

		;
		skiphash:
		if(!dohash)
		{
			serr("\n");serr("*** *** skiphash\n");
			iter_reset();
			for(tbs.iter = 0; tbs.iter < iters; tbs.iter++)
			{
				auto r = f(tbs);
				if(r.length == 4)
				{
					u4 h = *rc<u4*>(r.data);
					tbs.hash(h);
				}
				else
				{
					u8 h = *rc<u8*>(r.data);
					tbs.hash(h);
				}
			}
			tbs.print_entry("skiphash");
			serr("-------------------------------------------------------------------------------\n");
		}

		//
		serr("\n");
		tbs.print_end();
	}

	HashTestbed()
	{
		sout("%s",
R"(

https://github.com/lostdj/hashtestbed

```sh
git clone --recursive https://github.com/lostdj/hashtestbed
cd hashtestbed
clang++ -Wall -Wextra -O2 -mavx -maes -std=c++2a -o hashtestbed hashtestbed.cpp \
  && time ./hashtestbed 2>/dev/null >index.htm
```

)"
		);

		sout("* TODO: Hash of a hash.  \n");
		sout("* TODO: Visualizations via some js plot libs.  \n");
		sout("* TODO: Score-wise currently it's worse going above some %% with more buckets compared to another function going above some %% even more but within only one bucket. Fixme.  \n");
		sout("* TODO: Currently altscore is also broken.  \n");
		sout("  \n");

		sout("* '^ X': same function as above it, but xor hi and lo parts of the output accommodating the table size.  \n");
		sout("* Score: going 'above' punished more than going 'below'.  \n");
		sout("* Altscore: the theoretical metric from 'Red Dragon Book'. 1.0 is the best result. 'above' > 1, 'below' < 1.  \n");
		sout("* Collisions: hash func output collisions, not index. \n  * Might not matter much at low rates if used only for indexing into the table like here in these tests -- depends on how you're resolving it in your hash table. \n  * Note: currently the collision map takes into account only 4 lo bytes of output, that's where the high number of collisions might be coming from in the first place. Maybe. \n  * Native 4-byte hash funcs demonstrate their superiority here. \n  * t1ha is of a particular interest, as despite not being 4-byte native func, it has better distribution. \n  * Also there are exactly 9 dups in dic_ip.  \n");
		sout("* 'above' and 'below': buckets filled up above and below perfect 100%% respectively.  \n");
		sout("* 'ok': buckets filled up to the perfect 100%%.  \n");
		sout("* 'zero': buckets with no entries.  \n");
		sout("* '<%%' and '>%%': number of buckets going below and above some %%, so 25% < N <= 50%%. \n  * Less than 25%% will ***not*** put you in less than 50%%, etc. There is no <%% 0, see 'zero' instead.  \n");
		sout("    \n\n");
	}
}; // HashTestbed

//
int
main(int, char**)
{
	//
	{
		sout("<meta charset=\"utf-8\">\n\n");

		HashTestbed tbs;

		//
		auto mymmap = [](const char* p)
		{
			int fd = open(p, O_RDONLY, (mode_t)0600);
			if (fd == -1)
					serr("Error opening file '%s'", p),
					exit(EXIT_FAILURE);

			auto m = (byte*)mmap(0, 16ul * 1024 * 1024, PROT_READ, MAP_SHARED, fd, 0);
			if(m == MAP_FAILED)
				serr("Error mmaping file '%s'", p),
				exit(EXIT_FAILURE);

			return m;
		};

		const byte *const dic_common_words = mymmap("dic_common_words.txt");
		const byte *const dic_fr = mymmap("dic_fr.txt");
		const byte *const dic_ip = mymmap("dic_ip.txt");
		const byte *const dic_numbers = mymmap("dic_numbers.txt");
		const byte *const dic_postfix = mymmap("dic_postfix.txt");
		const byte *const dic_prefix = mymmap("dic_prefix.txt");
		const byte *const dic_Shakespeare = mymmap("dic_Shakespeare.txt");
		const byte *const dic_variables = mymmap("dic_variables.txt");
		const byte *const dic_win32 = mymmap("dic_win32.txt");
		const byte *const words_alpha = mymmap("words_alpha.txt");
		const byte *const words = mymmap("words.txt");

//		if(0)
		{
			sout("### Sanity / simple example checks\n");

			auto f = [](HashTestbed& tbs)
			{
				u4* d = (u4*)tbs.buf;
				*d = tbs.iter;

				return HashTestbed::Res{.data = (byte*)d, 4};
			};
			tbs.test("Sanity 1 iter 0..N-", 128, 64, f, false);
			tbs.test("Sanity 2 iter 0..N", 128, 128, f, false);
			tbs.test("Sanity 3 iter 0..N+", 128, 256, f, false);
		}

		if(0)
		{
			sout("### Mapping PCG PRNG output, no hashing\n");

			cexpr const auto randreset = false;

			static pcg::Pcg pcg(0ul);

			auto f = [&](HashTestbed& tbs)
			{
				if(randreset && tbs.iter == 0)
					pcg = pcg::Pcg(0ul);

				u4* d = (u4*)tbs.buf;
				*d = pcg.next4();

				return HashTestbed::Res{.data = (byte*)d, 4};
			};
			tbs.test("Rnd - PCG84", 128, 64, f, false);
			tbs.test("Rnd - PCG84", 128, 128, f, false);
			tbs.test("Rnd - PCG84", 128, 256, f, false);
			tbs.test("Rnd - PCG84", 128, 512, f, false);
			tbs.test("Rnd - PCG84", 128, 1024, f, false);
			tbs.test("Rnd - PCG84", 128, 2048, f, false);
			tbs.test("Rnd - PCG84", 128, 4096, f, false);

			tbs.test("Rnd - PCG84", 4096, 128, f, false);
			tbs.test("Rnd - PCG84", 4096, 256, f, false);
			tbs.test("Rnd - PCG84", 4096, 512, f, false);
			tbs.test("Rnd - PCG84", 4096, 1024, f, false);
			tbs.test("Rnd - PCG84", 4096, 2048, f, false);
			tbs.test("Rnd - PCG84", 500000, 1005000, f, false);
			auto pcgstate = pcg.state;tbs.test("Rnd - PCG84", 4096, 4096, f, false);

			auto f_bound1 = [&](HashTestbed& tbs)
			{
				u4* d = (u4*)tbs.buf;
				*d = pcg.next4_v1(4097);

				return HashTestbed::Res{.data = (byte*)d, 4};
			};
			pcg.state = pcgstate;tbs.test("Rnd - PCG84 - bound1", 4096, 4096, f_bound1, false);
			auto f_bound2 = [&](HashTestbed& tbs)
			{
				u4* d = (u4*)tbs.buf;
				*d = pcg.next4_v2(4097);

				return HashTestbed::Res{.data = (byte*)d, 4};
			};
			pcg.state = pcgstate;tbs.test("Rnd - PCG84 - bound2", 4096, 4096, f_bound2, false);
		}

		if(0)
		{
			sout("### Mapping %%your%% std PRNG output, no hashing\n");

			cexpr const auto randreset = false;

			srand(1337);

			auto f = [&](HashTestbed& tbs)
			{
				if(randreset && tbs.iter == 0)
					srand(1337);

				u4* d = (u4*)tbs.buf;
				*d = rand();

				return HashTestbed::Res{.data = (byte*)d, 4};
			};
			tbs.test("Rnd - STD4", 128, 64, f, false);
			tbs.test("Rnd - STD4", 128, 128, f, false);
			tbs.test("Rnd - STD4", 128, 256, f, false);
			tbs.test("Rnd - STD4", 128, 512, f, false);
			tbs.test("Rnd - STD4", 128, 1024, f, false);
			tbs.test("Rnd - STD4", 128, 2048, f, false);
			tbs.test("Rnd - STD4", 128, 4096, f, false);

			tbs.test("Rnd - STD4", 4096, 128, f, false);
			tbs.test("Rnd - STD4", 4096, 256, f, false);
			tbs.test("Rnd - STD4", 4096, 512, f, false);
			tbs.test("Rnd - STD4", 4096, 1024, f, false);
			tbs.test("Rnd - STD4", 4096, 2048, f, false);
			tbs.test("Rnd - PCG84", 4096, 4096, f, false);
		}

		//if(0)
		{
			sout("### Hash(iter 0..N)\n");

			auto f = [](HashTestbed& tbs)
			{
				u4* d = (u4*)tbs.buf;
				*d = tbs.iter;

				return HashTestbed::Res{.data = (byte*)d, 4};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 5000, f);
			tbs.test("0..N+", 256, 1005000, f);
		}

		//if(0)
		{
			sout("### Hash(rnd)\n");

			cexpr const auto randreset = true;

			static pcg::Pcg pcg(0ul);

			auto f = [&](HashTestbed& tbs)
			{
				if(randreset && tbs.iter == 0)
					pcg = pcg::Pcg(0ul);

				u4* d = (u4*)tbs.buf;
				*d = pcg.next4();

				return HashTestbed::Res{.data = (byte*)d, 4};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 5000, f);
			tbs.test("0..N+", 256, 1005000, f);
		}

		//if(0)
		{
			sout("### Hash(dic_common_words)\n");

			const byte* map = dic_common_words;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_common_words;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 500, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("1:1", 500, 500, f);
			tbs.test("0..N-", 100000, 500, f);
			tbs.test("0..N-", 370000, 500, f);
		}

		//if(0)
		{
			sout("### Hash(dic_fr)\n");

			const byte* map = dic_fr;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_fr;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("0..N+", 10000, 13400, f);
			tbs.test("1:1", 13400, 13400, f);
			tbs.test("0..N-", 100000, 13400, f);
			tbs.test("0..N-", 370000, 13400, f);
		}

		//if(0)
		{
			sout("### Hash(dic_ip)\n");

			const byte* map = dic_ip;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_ip;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("0..N+", 3000, 3900, f);
			tbs.test("1:1", 3900, 3900, f);
			tbs.test("0..N-", 100000, 3900, f);
			tbs.test("0..N-", 370000, 3900, f);
		}

		//if(0)
		{
			sout("### Hash(dic_numbers)\n");

			const byte* map = dic_numbers;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_numbers;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 500, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("1:1", 500, 500, f);
			tbs.test("0..N-", 100000, 500, f);
			tbs.test("0..N-", 370000, 500, f);
		}

		//if(0)
		{
			sout("### Hash(dic_postfix)\n");

			const byte* map = dic_postfix;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_postfix;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 100, 500, f);
			tbs.test("0..N+", 128, 500, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("1:1", 500, 500, f);
			tbs.test("0..N-", 100000, 500, f);
			tbs.test("0..N-", 370000, 500, f);
		}

		//if(0)
		{
			sout("### Hash(dic_prefix)\n");

			const byte* map = dic_prefix;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_prefix;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 500, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("1:1", 500, 500, f);
			tbs.test("0..N-", 100000, 500, f);
			tbs.test("0..N-", 370000, 500, f);
		}

		//if(0)
		{
			sout("### Hash(dic_Shakespeare)\n");

			const byte* map = dic_Shakespeare;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_Shakespeare;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("1:1", 3200, 3200, f);
			tbs.test("0..N-", 100000, 3200, f);
			tbs.test("0..N-", 370000, 3200, f);
		}

		//if(0)
		{
			sout("### Hash(dic_variables)\n");

			const byte* map = dic_variables;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_variables;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("1:1", 1800, 1800, f);
			tbs.test("0..N-", 100000, 1800, f);
			tbs.test("0..N-", 370000, 1800, f);
		}

		//if(0)
		{
			sout("### Hash(dic_win32)\n");

			const byte* map = dic_win32;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = dic_win32;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 500, f);
			tbs.test("1:1", 1900, 1900, f);
			tbs.test("0..N-", 100000, 1900, f);
			tbs.test("0..N-", 370000, 1900, f);
		}

		//if(0)
		{
			sout("### Hash(words_alpha)\n");

			const byte* map = words_alpha;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = words_alpha;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 5000, f);
			tbs.test("0..N+", 256, 370000, f);
			tbs.test("0..N+", 100000, 370000, f);
			tbs.test("1:1", 370000, 370000, f);
			tbs.test("0..N-", 500000, 370000, f);
		}

		//if(0)
		{
			sout("### Hash(words)\n");

			const byte* map = words;

			auto f = [&](HashTestbed& tbs)
			{
				if(tbs.iter == 0)
					map = words;

				u8 l = 0;
				while(map[l] && map[l] != '\r' && map[l] != '\n')
					++l;

//				serr("WORDS WORDS %p %.*s WORDS WORDS\n", map, l, map);

				auto res = map;
				map += l + 1;
				if(*map == '\n')
					++map;

				return HashTestbed::Res{.data = (byte*const)res, l};
			};
			tbs.test("0..N-", 8, 3, f);
			tbs.test("0..N-", 8, 7, f);
			tbs.test("0..N", 8, 8, f);
			tbs.test("0..N+", 8, 9, f);
			tbs.test("0..N+", 8, 16, f);
			tbs.test("0..N+", 128, 128, f);
			tbs.test("0..N+", 128, 256, f);
			tbs.test("0..N+", 128, 512, f);
			tbs.test("0..N+", 128, 1024, f);
			tbs.test("0..N-", 256, 100, f);
			tbs.test("0..N+", 256, 300, f);
			tbs.test("0..N+", 256, 5000, f);
			tbs.test("0..N+", 256, 466000, f);
			tbs.test("0..N+", 100000, 466000, f);
			tbs.test("0..N+", 200000, 466000, f);
			tbs.test("1:1", 466000, 466000, f);
			tbs.test("0..N-", 500000, 466000, f);
			tbs.test("0..N-", 1000000, 466000, f);
		}

		sout("\n\n<script src=\"bloat/js.js\"></script>\n");
	}

	return 0;
}
