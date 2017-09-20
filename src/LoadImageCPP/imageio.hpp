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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#include <Windows.h>
#include <GdiPlus.h>

#endif

#include "imageformats.hpp"

class ImageIO
{
public:

	static GrayscaleFloatImage FileToGrayscaleFloatImage(const char* filename);
	static GrayscaleByteImage FileToGrayscaleByteImage(const char* filename);
	static ColorFloatImage FileToColorFloatImage(const char* filename);
	static ColorByteImage FileToColorByteImage(const char* filename);

	static void ImageToFile(const GrayscaleFloatImage &image, const char *filename);
	static void ImageToFile(const GrayscaleByteImage &image, const char *filename);
	static void ImageToFile(const ColorFloatImage &image, const char *filename);
	static void ImageToFile(const ColorByteImage &image, const char *filename);


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

private:
	static GrayscaleFloatImage BitmapToGrayscaleFloatImage(Gdiplus::Bitmap &B);
	static GrayscaleByteImage BitmapToGrayscaleByteImage(Gdiplus::Bitmap &B);
	static ColorFloatImage BitmapToColorFloatImage(Gdiplus::Bitmap &B);
	static ColorByteImage BitmapToColorByteImage(Gdiplus::Bitmap &B);

	static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const GrayscaleFloatImage &image);
	static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const GrayscaleByteImage &image);
	static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const ColorFloatImage &image);
	static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const ColorByteImage &image);

#endif
};
