#pragma once

#include <cstdint>

union wide_char_t
{
	struct
	{
		char upper;
		char lower;
	};

	wchar_t wide;
};

// Vectors
template<typename T>
struct vector2
{
	T x, y;
};

template<typename T>
struct vector3  
{ 
	T x, y, z;
};

template<typename T>
struct vector4
{ 
	T x, y, z, w;
};

// Color vectors
struct rgb
{ 
	uint8_t r, g, b; 
};

struct rgba
{ 
	uint8_t r, g, b, a; 
};