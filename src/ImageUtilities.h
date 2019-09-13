#pragma once

#include <FreeImage.h>

#include <QString>
#include <QSize>

FIBITMAP* freeImageLoad(const QString& imageFilePath);
FIMULTIBITMAP* freeImageOpenMultiBitmap(const QString& imageFilePath);
int freeImageGetPageCount(const QString& imageFilePath);
QSize freeImageGetSize(const QString& imageFilePath);