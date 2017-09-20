#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#include <Windows.h>	// Windows.h must be included before GdiPlus.h
#include <GdiPlus.h>

#pragma comment(lib, "gdiplus.lib")

ULONG_PTR m_gdiplusToken;   // class member

#endif

#include "imageformats.hpp"
#include "imageio.hpp"

float ToGray(ColorFloatPixel pixel)
{
	return pixel.b * 0.114f + pixel.g * 0.587f + pixel.r * 0.299f;
}

void TestFunc(char *inputfilename, char *outputfilename)
{
	ColorFloatImage image = ImageIO::FileToColorFloatImage(inputfilename);
	GrayscaleFloatImage res(image.Width(), image.Height());
	
	for (int j = 0; j < res.Height(); j++)
		for (int i = 0; i < res.Width(); i++)
			res(i, j) = ToGray(image(res.Width() - 1 - i, j));

	ImageIO::ImageToFile(res, outputfilename);
}

int main_func(int argc, char* argv[])
{
	// Put your code here

	if (argc < 3)
		return 0;

	TestFunc(argv[1], argv[2]);

	return 0;
}

// wchar_t - UTF16 character
// wmain - entry point for wchar_t arguments
// Note: changed to UTF-8 due to multiplatform compatibility
int main(int argc, char* argv[])
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

	 // InitInstance
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	int exit_code;

	try
	{
		exit_code = main_func(argc, argv);
	}
	catch (...)
	{
		exit_code = -1;
	}

	// ExitInstance
    Gdiplus::GdiplusShutdown(m_gdiplusToken);

	return exit_code;
#else
	return main_func(argc, argv);
#endif
}

