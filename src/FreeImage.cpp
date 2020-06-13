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
	const auto format = fi::FreeImage_GetFileType(utf8);

	return FreeImage_Load(format, utf8);
#endif
}