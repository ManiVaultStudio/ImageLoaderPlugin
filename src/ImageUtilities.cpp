#include "ImageUtilities.h"

#include <QDebug>

FIBITMAP* freeImageLoad(const QString & imageFilePath)
{
#ifdef _WIN32
	const void* const voidPtr = imageFilePath.utf16();
	const auto wcharPtr = static_cast<const wchar_t*>(voidPtr);
	const auto format = FreeImage_GetFileTypeU(wcharPtr);

	return FreeImage_LoadU(format, wcharPtr);
#else
	const auto utf8 = imageFilePath.toUtf8();
	const auto format = FreeImage_GetFileType(utf8);

	return FreeImage_Load(format, utf8);
#endif
}

FIMULTIBITMAP* freeImageOpenMultiBitmap(const QString& imageFilePath)
{
#ifdef _WIN32
#else
#endif

	const auto fileNameUtf8 = imageFilePath.toUtf8();
	const auto format = FreeImage_GetFileType(fileNameUtf8);

	return FreeImage_OpenMultiBitmap(FIF_TIFF, fileNameUtf8, true, false, false);
}

int freeImageGetPageCount(const QString& imageFilePath)
{
	FIMULTIBITMAP* multiBitmap = FreeImage_OpenMultiBitmap(FIF_TIFF, imageFilePath.toUtf8(), false, false);

	if (multiBitmap == nullptr)
		return -1;

	const auto pageCount = FreeImage_GetPageCount(multiBitmap);

	FreeImage_CloseMultiBitmap(multiBitmap);

	return pageCount;
}

QSize freeImageGetSize(const QString & imageFilePath)
{
	FIBITMAP* bitmap = freeImageLoad(imageFilePath);

	const auto bitmapSize = QSize(FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap));

	FreeImage_Unload(bitmap);

	return bitmapSize;
}
