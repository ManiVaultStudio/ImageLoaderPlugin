#pragma once

#include <QString>
#include <QSize>

namespace fi {
#include <FreeImage.h>
}

class FreeImage
{
public:
	static fi::FIBITMAP* freeImageLoad(const QString& imageFilePath);
	static fi::FIMULTIBITMAP* freeImageOpenMultiBitmap(const QString& imageFilePath);
	static int freeImageGetPageCount(const QString& imageFilePath);
	static QSize freeImageGetSize(const QString& imageFilePath);
};