#include "FreeImage.h"

#include <QDebug>

fi::FIBITMAP* FreeImage::freeImageLoad(const QString & imageFilePath)
{
#ifdef _WIN32
	const void* const voidPtr = imageFilePath.utf16();
	const auto wcharPtr = static_cast<const wchar_t*>(voidPtr);
	const auto format = fi::FreeImage_GetFileTypeU(wcharPtr);

	return fi::FreeImage_LoadU(format, wcharPtr);
#else
	const auto utf8 = imageFilePath.toUtf8();
	const auto format = FreeImage_GetFileType(utf8);

	return FreeImage_Load(format, utf8);
#endif
}

fi::FIMULTIBITMAP* FreeImage::freeImageOpenMultiBitmap(const QString& imageFilePath)
{
#ifdef _WIN32
#else
#endif

	const auto fileNameUtf8 = imageFilePath.toUtf8();
	const auto format = fi::FreeImage_GetFileType(fileNameUtf8);

	return fi::FreeImage_OpenMultiBitmap(fi::FIF_TIFF, fileNameUtf8, false, false, false);
}

int FreeImage::freeImageGetPageCount(const QString& imageFilePath)
{
	fi::FIMULTIBITMAP* multiBitmap = fi::FreeImage_OpenMultiBitmap(fi::FIF_TIFF, imageFilePath.toUtf8(), false, false);

	if (multiBitmap == nullptr)
		return -1;

	const auto pageCount = fi::FreeImage_GetPageCount(multiBitmap);

	fi::FreeImage_CloseMultiBitmap(multiBitmap);

	return pageCount;
}

QSize FreeImage::freeImageGetSize(const QString & imageFilePath)
{
	fi::FIBITMAP* bitmap = FreeImage::freeImageLoad(imageFilePath);

	const auto bitmapSize = QSize(fi::FreeImage_GetWidth(bitmap), fi::FreeImage_GetHeight(bitmap));

	fi::FreeImage_Unload(bitmap);

	return bitmapSize;
}
