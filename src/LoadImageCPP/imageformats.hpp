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

#include <memory>
#include <assert.h>
#include "pixelformats.hpp"

template<typename PixelType>
class ImageBase
{
private:
	std::unique_ptr<PixelType[]> rawdata;
	int width, height;

public:
	inline ImageBase(int Width, int Height)
	{
		assert(Width > 0 && Height > 0);

		this->width = Width;
		this->height = Height;
		this->rawdata.reset(new PixelType[Width * Height]);
	}

	// --- The code is needed for C++11 compatibility (for VS2013) ---

	ImageBase(const ImageBase&) = default;
	ImageBase(ImageBase&&) = default;

	ImageBase& operator = (const ImageBase&) = default;
	ImageBase& operator = (ImageBase&&) = default;

	// --- End of code region ---

	inline PixelType operator() (int x, int y) const
	{
		assert(x >= 0 && x < width && y >= 0 && y < height);
		return rawdata[y * width + x];
	}

	inline PixelType& operator() (int x, int y)
	{
		assert(x >= 0 && x < width && y >= 0 && y < height);
		return rawdata[y * width + x];
	}

	inline int Width() const
	{
		return width;
	}

	inline int Height() const
	{
		return height;
	}

	inline ImageBase<PixelType> Copy() const
	{
		ImageBase<PixelType> res(width, height);

		if (rawdata)
			memcpy(res.rawdata.get(), rawdata.get(), width * height * sizeof(PixelType));

		return res;
	}
};

typedef ImageBase<unsigned char> GrayscaleByteImage;
typedef ImageBase<ColorBytePixel> ColorByteImage;
typedef ImageBase<float> GrayscaleFloatImage;
typedef ImageBase<ColorFloatPixel> ColorFloatImage;
