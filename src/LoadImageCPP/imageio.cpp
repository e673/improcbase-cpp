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

#include "imageio.hpp"

#include <vector>

// =======================================================================================================

class LockedBitmap
{
public:
	LockedBitmap(Gdiplus::Bitmap &B);
	LockedBitmap(Gdiplus::Bitmap &B, Gdiplus::PixelFormat pf, int pixelsize);
	~LockedBitmap();

	int Width() const;
	int Height() const;
	int Stride() const;
	void *Data() const;

private:
	Gdiplus::Bitmap &B;
	Gdiplus::BitmapData bitmapdata;
};

// =======================================================================================================

LockedBitmap::LockedBitmap(Gdiplus::Bitmap &B)
	: LockedBitmap(B, PixelFormat32bppRGB, 4) {}

LockedBitmap::LockedBitmap(Gdiplus::Bitmap &B, Gdiplus::PixelFormat pf, int pixelsize)
	: B(B)
{
	Gdiplus::RectF boundsF;
	Gdiplus::Unit unit = Gdiplus::UnitPixel;
	B.GetBounds(&boundsF, &unit);

	Gdiplus::Rect bounds((int)boundsF.X, (int)boundsF.Y, (int)boundsF.Width, (int)boundsF.Height);
	B.LockBits(&bounds, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, pf, &bitmapdata);
}

LockedBitmap::~LockedBitmap()
{
	B.UnlockBits(&bitmapdata);
}

int LockedBitmap::Width() const { return bitmapdata.Width; }
int LockedBitmap::Height() const { return bitmapdata.Height; }
int LockedBitmap::Stride() const { return bitmapdata.Stride; }
void* LockedBitmap::Data() const { return bitmapdata.Scan0; }

// =======================================================================================================

static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

// =======================================================================================================

template <typename PixelType, class Converter>
static ImageBase<PixelType> BitmapToImage(Gdiplus::Bitmap &B, Converter conv)
{
	int W = B.GetWidth(), H = B.GetHeight();
	ImageBase<PixelType> res(W, H);

	if (B.GetPixelFormat() == PixelFormat8bppIndexed)
	{
		int pal_size = B.GetPaletteSize();	// Get palette size in bytes
		Gdiplus::ColorPalette *palette = (Gdiplus::ColorPalette*)malloc(pal_size);
		B.GetPalette(palette, pal_size);
		LockedBitmap lbi(B, PixelFormat8bppIndexed, 1);

		for (int j = 0; j < lbi.Height(); j++)
		{
			unsigned char *line = (unsigned char*)lbi.Data() + j * lbi.Stride();
			for (int i = 0; i < lbi.Width(); i++)
			{
				unsigned char c = line[i];
				if (c >= palette->Count)
					continue;
				
				Gdiplus::ARGB color = palette->Entries[c];
				unsigned char b = (color & 0xFF);
				unsigned char g = ((color >> 8) & 0xFF);
				unsigned char r = ((color >> 16) & 0xFF);

				res(i, j) = conv(ColorBytePixel(b, g, r));
			}
		}
	}
	else
	{
		LockedBitmap lbi(B);
		for (int j = 0; j < lbi.Height(); j++)
		{
			unsigned char *line = (unsigned char*)lbi.Data() + j * lbi.Stride();
			for (int i = 0; i < lbi.Width(); i++)
			{
				unsigned char b = line[i * 4];
				unsigned char g = line[i * 4 + 1];
				unsigned char r = line[i * 4 + 2];
				
				res(i, j) = conv(ColorBytePixel(b, g, r));
			}
		}
	}

	return res;
}

template <typename PixelType, class Converter>
static std::unique_ptr<Gdiplus::Bitmap> ImageToBitmap(const ImageBase<PixelType> &image, Converter conv)
{
	std::unique_ptr<Gdiplus::Bitmap> B(new Gdiplus::Bitmap(image.Width(), image.Height(), PixelFormat24bppRGB));
	LockedBitmap lbi(*B);

	for (int j = 0; j < image.Height(); j++)
	{
		unsigned char *line = (unsigned char*)lbi.Data() + j * lbi.Stride();
		for (int i = 0; i < image.Width(); i++)
		{
			ColorBytePixel c = conv(image(i, j));
			line[i * 4] = c.b;
			line[i * 4 + 1] = c.g;
			line[i * 4 + 2] = c.r;
		}
	}
	return B;
}

// -----------------------------------------------------------------------------------------------

GrayscaleFloatImage ImageIO::BitmapToGrayscaleFloatImage(Gdiplus::Bitmap &B)
{
	return BitmapToImage<float>(B, [](ColorBytePixel p) { return 0.114f * p.b + 0.587f * p.g + 0.299f * p.r; });
}

GrayscaleByteImage ImageIO::BitmapToGrayscaleByteImage(Gdiplus::Bitmap &B)
{
	return BitmapToImage<unsigned char>(B, [](ColorBytePixel p) { return (unsigned char)(0.114f * p.b + 0.587f * p.g + 0.299f * p.r); });
}

ColorFloatImage ImageIO::BitmapToColorFloatImage(Gdiplus::Bitmap &B)
{
	return BitmapToImage<ColorFloatPixel>(B, [](ColorBytePixel p) { return ColorFloatPixel(p.b, p.g, p.r); });
}

ColorByteImage ImageIO::BitmapToColorByteImage(Gdiplus::Bitmap &B)
{
	return BitmapToImage<ColorBytePixel>(B, [](ColorBytePixel p) { return p; });
}

// -----------------------------------------------------------------------------------------------

GrayscaleFloatImage ImageIO::FileToGrayscaleFloatImage(const wchar_t* filename)
{
	Gdiplus::Bitmap B(filename);
	return BitmapToGrayscaleFloatImage(B);
}

GrayscaleByteImage ImageIO::FileToGrayscaleByteImage(const wchar_t* filename)
{
	Gdiplus::Bitmap B(filename);
	return BitmapToGrayscaleByteImage(B);
}

ColorFloatImage ImageIO::FileToColorFloatImage(const wchar_t* filename)
{
	Gdiplus::Bitmap B(filename);
	return BitmapToColorFloatImage(B);
}

ColorByteImage ImageIO::FileToColorByteImage(const wchar_t* filename)
{
	Gdiplus::Bitmap B(filename);
	return BitmapToColorByteImage(B);
}

// -----------------------------------------------------------------------------------------------

static inline unsigned char f2b(float x)
{
	if (x < 0.0f)
		return 0;
	else if (x > 255.0f)
		return 255;
	else
		return (unsigned char)x;
}

std::unique_ptr<Gdiplus::Bitmap> ImageIO::ImageToBitmap(const GrayscaleFloatImage &image)
{
	return ::ImageToBitmap(image, [](float x)
	{
		unsigned char v = f2b(x);
		return ColorBytePixel(v, v, v);	
	});
}

std::unique_ptr<Gdiplus::Bitmap> ImageIO::ImageToBitmap(const GrayscaleByteImage &image)
{
	return ::ImageToBitmap(image, [](unsigned char x) { return ColorBytePixel(x, x, x); });
}

std::unique_ptr<Gdiplus::Bitmap> ImageIO::ImageToBitmap(const ColorFloatImage &image)
{
	return ::ImageToBitmap(image, [](ColorFloatPixel x) { return ColorBytePixel(f2b(x.b), f2b(x.g), f2b(x.r)); });
}

std::unique_ptr<Gdiplus::Bitmap> ImageIO::ImageToBitmap(const ColorByteImage &image)
{
	return ::ImageToBitmap(image, [](ColorBytePixel x) { return x; });
}

// -----------------------------------------------------------------------------------------------

void ImageIO::ImageToFile(const GrayscaleFloatImage &image, const wchar_t *filename)
{
	std::unique_ptr<Gdiplus::Bitmap> B = ImageToBitmap(image);
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	B->Save(filename, &pngClsid);

}

void ImageIO::ImageToFile(const GrayscaleByteImage &image, const wchar_t *filename)
{
	std::unique_ptr<Gdiplus::Bitmap> B = ImageToBitmap(image);
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	B->Save(filename, &pngClsid);
}

void ImageIO::ImageToFile(const ColorFloatImage &image, const wchar_t *filename)
{
	std::unique_ptr<Gdiplus::Bitmap> B = ImageToBitmap(image);
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	B->Save(filename, &pngClsid);
}

void ImageIO::ImageToFile(const ColorByteImage &image, const wchar_t *filename)
{
	std::unique_ptr<Gdiplus::Bitmap> B = ImageToBitmap(image);
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	B->Save(filename, &pngClsid);
}

// =======================================================================================================
