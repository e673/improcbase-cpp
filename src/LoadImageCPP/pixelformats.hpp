/*

Copyright (c) 2011-2016 Andrey Nasonov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#pragma once

#pragma pack(push, 1)

struct ColorBytePixel
{
	unsigned char b, g, r, a;

	inline ColorBytePixel()
		: b(0), g(0), r(0), a(0) {}

	inline ColorBytePixel(unsigned char b, unsigned char g, unsigned char r, unsigned char a = 0)
		: b(b), g(g), r(r), a(a) {}

};

struct ColorFloatPixel
{
public:

	float b, g, r, a;

	inline ColorFloatPixel()
		: b(0.0f), g(0.0f), r(0.0f), a(0.0f) {}

	inline ColorFloatPixel(float b, float g, float r, float a = 0.0f)
		: b(b), g(g), r(r), a(a) {}

	inline ColorFloatPixel& operator += (const ColorFloatPixel &other)
	{
		b += other.b;
		g += other.g;
		r += other.r;
		a += other.a;
		return *this;
	}

	inline ColorFloatPixel operator + (const ColorFloatPixel &other) const
	{
		return ColorFloatPixel(b + other.b, g + other.g, r + other.r, a + other.a);
	}

	inline ColorFloatPixel operator * (float q) const
	{
		return ColorFloatPixel(b * q, g * q, r * q, a * q);
	}
};

inline ColorFloatPixel operator * (float q, const ColorFloatPixel &p)
{
	return ColorFloatPixel(p.b * q, p.g * q, p.r * q, p.a * q);
}

#pragma pack(pop)

