#pragma once

#include <vector>

#include <QStringList>
#include <QMap>
#include <QSize>

enum class ImageResamplingFilter
{
	Box,
	Bilinear,
	BSpline,
	Bicubic,
	CatmullRom,
	Lanczos
};

using FloatVector = std::vector<float>;
using ImageList = QStringList;