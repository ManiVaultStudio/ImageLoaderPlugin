#pragma once

#include <vector>


#include <QStringList>

enum class ImageCollectionType
{
	Sequence,
	Stack,
	MultiPartSequence
};

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
