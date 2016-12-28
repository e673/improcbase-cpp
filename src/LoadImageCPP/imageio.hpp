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

#include <Windows.h>
#include <GdiPlus.h>

#include "imageformats.hpp"

class ImageIO
{
public:
	static GrayscaleFloatImage BitmapToGrayscaleFloatImage(Gdiplus::Bitmap &B);
	static GrayscaleByteImage BitmapToGrayscaleByteImage(Gdiplus::Bitmap &B);
	static ColorFloatImage BitmapToColorFloatImage(Gdiplus::Bitmap &B);
	static ColorByteImage BitmapToColorByteImage(Gdiplus::Bitmap &B);

	static GrayscaleFloatImage FileToGrayscaleFloatImage(const wchar_t* filename);
	static GrayscaleByteImage FileToGrayscaleByteImage(const wchar_t* filename);
	static ColorFloatImage FileToColorFloatImage(const wchar_t* filename);
	static ColorByteImage FileToColorByteImage(const wchar_t* filename);

	static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const GrayscaleFloatImage &image);
	static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const GrayscaleByteImage &image);
	static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const ColorFloatImage &image);
	static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const ColorByteImage &image);

	static void ImageToFile(const GrayscaleFloatImage &image, const wchar_t *filename);
	static void ImageToFile(const GrayscaleByteImage &image, const wchar_t *filename);
	static void ImageToFile(const ColorFloatImage &image, const wchar_t *filename);
	static void ImageToFile(const ColorByteImage &image, const wchar_t *filename);
};
