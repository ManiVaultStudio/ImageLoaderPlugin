#pragma once

#include <vector>


#include <QStringList>

enum class ImageCollectionType
{
	Sequence,
	Stack,
	MultiPartSequence
};

inline QString imageCollectionTypeName(const ImageCollectionType& type)
{
	switch (type)
	{
	case ImageCollectionType::Sequence:
		return "Sequence";

	case ImageCollectionType::Stack:
		return "Stack";

	case ImageCollectionType::MultiPartSequence:
		return "MultiPartSequence";

	default:
		break;
	}

	return "";
}

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